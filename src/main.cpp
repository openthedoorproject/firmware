#include "System.h"

#include "Keyboard/Keyboard.h"

#include "Door/Door.h"
#include "Display/Display.h"
#include "Card/CardReader.h"

#include "Auth/PasswordParser.h"
#include "Auth/Auth.h"

enum State : int
{
    WAITING = 0,
    STARTING = 1,

    READING_PASSWORD = STARTING << 1,
    READING_CARD = READING_PASSWORD << 1,
    HANDLING_INPUT = READING_CARD | READING_PASSWORD,

    LED_YELLOW = READING_CARD << 1,
    LED_RED = LED_YELLOW << 1,
    LED_GREEN = LED_RED << 1,
    LEDS_OFF = LED_GREEN << 1,
    LEDS_ON = LEDS_OFF << 1,

    VERIFY_PASSWORD = LED_GREEN << 1,
    VERIFY_CARD = VERIFY_PASSWORD << 1,
    REQUESTING_AUTH = VERIFY_CARD | VERIFY_PASSWORD,

    DENY_ACCESS = VERIFY_CARD << 1,
    GRANT_ACCESS = DENY_ACCESS << 1,

    PERFORMING_OPERATION = HANDLING_INPUT | REQUESTING_AUTH | DENY_ACCESS | GRANT_ACCESS
};

static bool EXIT_BUTTON_PRESSED = false;

State state = STARTING;
bool stateChanged = false;

#define ONCE_PER_STATE(handler) \
    if (stateChanged)           \
    handler

#define INTERVAL(control, timeout, action) \
    if (millis() - control > timeout)      \
    action

inline void SetState(State state)
{
    stateChanged = stateChanged || state != ::state;
    ::state = state;
}

inline bool HasState(State search)
{
    return ((int)state) & ((int)search);
}

inline void ToggleState(State targetState)
{
    SetState(static_cast<State>(((int)::state) ^ ((int)targetState)));
}

inline void AddState(State state)
{
    SetState(static_cast<State>(((int)::state) | ((int)state)));
}

inline void RemoveState(State state)
{
    LOG_INFO("removing state: " + String((int)state));
    SetState(static_cast<State>(((int)::state) & (~((int)state))));
}

inline bool IsWaiting()
{
    return ::state == WAITING;
}

void setup()
{
    LOG_INIT(9600);
    Door::Init(4, 17);
    Door::SetOpenTimeout(7000);

    Keyboard::Init(4, 4, new int[4]{12, 14, 27, 26}, new int[4]{25, 33, 32, 5});

    Keyboard::SetRow(0, new char[4]{'1', '2', '3', 'A'});
    Keyboard::SetRow(1, new char[4]{'4', '5', '6', 'B'});
    Keyboard::SetRow(2, new char[4]{'7', '8', '9', 'C'});
    Keyboard::SetRow(3, new char[4]{'*', '0', '#', 'D'});

    PasswordParser::SetTimeout(4000);
    PasswordParser::SetPasswordSize(4);

    Display::Init(0x27, 2, 16);

    CardReader::Init(16, 14);

    Auth::Init(AUTH_API_URL, LAB_NUMBER, WIFI_SSID, WIFI_PASSWORD);

    SetState(WAITING);

    LOG_INFO("setup finished");
}

static void wait()
{
    ONCE_PER_STATE(Display::Clear());
    ONCE_PER_STATE(Display::Write("Open the door"));

    ONCE_PER_STATE(LOG_INFO("open the door: waiting"));
}

static void processPassword()
{
    PasswordParser::ParsingState parsingState = PasswordParser::Parse();

    ONCE_PER_STATE(Display::Clear());
    ONCE_PER_STATE(Display::Write("Insira a senha"));
    ONCE_PER_STATE(Display::Cursor(1, 0));

    ONCE_PER_STATE(LOG_INFO("insira a senha"));

    if (parsingState & PasswordParser::ENDED)
    {
        if (parsingState == PasswordParser::ERROR)
        {
            LOG_WARN("error while parsing password");
            AddState(DENY_ACCESS);
        }
        else
        {
            LOG_INFO("parsing password finished");
            AddState(VERIFY_PASSWORD);
        }
        RemoveState(READING_PASSWORD);
    }
    else if (parsingState & PasswordParser::ACCEPTED)
    {
        Display::Write('*');
    }
}

static void verifyPassword()
{
    String password = PasswordParser::GetParsedPassword();

    ONCE_PER_STATE(Display::Clear());
    ONCE_PER_STATE(Display::Write("Verificando..."));

    ONCE_PER_STATE(LOG_INFO("verificando..."));

    if (Auth::VerifyPassword(password))
    {
        LOG_INFO("password accepted");
        AddState(GRANT_ACCESS);
    }
    else
    {
        LOG_INFO("invalid password");
        AddState(DENY_ACCESS);
    }

    RemoveState(VERIFY_PASSWORD);
}

static void processCard()
{
    if (CardReader::ReadCard() == CardReader::ERROR)
    {
        LOG_ERROR("fail to read card");
        AddState(DENY_ACCESS);
    }
    else
    {
        AddState(VERIFY_CARD);
        LOG_INFO("card read: " + CardReader::GetCardUID().HexString());
    }

    RemoveState(READING_CARD);
}

static void verifyCard()
{
    CardReader::UID inputUID = CardReader::GetCardUID();

    ONCE_PER_STATE(Display::Clear());
    ONCE_PER_STATE(Display::Write("Verificando..."));

    ONCE_PER_STATE(LOG_INFO("Verificando..."));

    if (Auth::VerifyCard(inputUID.HexString()))
    {
        AddState(GRANT_ACCESS);
    }
    else
    {
        LOG_INFO("invalid card");
        AddState(DENY_ACCESS);
    }

    RemoveState(VERIFY_CARD);
}

static void denyAccess()
{
    static unsigned long requestTime;

    ONCE_PER_STATE(Display::Clear());
    ONCE_PER_STATE(Display::Write("Acesso negado"));

    ONCE_PER_STATE(requestTime = millis());
    ONCE_PER_STATE(LOG_INFO("acesso negado"));

    INTERVAL(requestTime, 3000, RemoveState(DENY_ACCESS));
}

static void grantAccess()
{
    ONCE_PER_STATE({
        Display::Clear();
        Display::Write("Acesso permitido");
        LOG_INFO("acesso permitido");

        if (EXIT_BUTTON_PRESSED)
        {
            Door::IncreaseTimer();
        }
    });

    if (!Door::GrantAccess())
        RemoveState(GRANT_ACCESS);
}

static void buttonPressed()
{
    ONCE_PER_STATE({
        if (HasState(GRANT_ACCESS))
        {
            Door::IncreaseTimer();
        }
    });

    if (!Door::GrantAccess())
        EXIT_BUTTON_PRESSED = false;
}

static void checkInputs()
{
    if (!HasState(PERFORMING_OPERATION))
    {
        if (Keyboard::HasInput())
        {
            AddState(READING_PASSWORD);
            LOG_INFO("keyboard input found");
        }
        else if (CardReader::HasCard())
        {
            AddState(READING_CARD);
            LOG_INFO("card found");
        }
    }

    if (Door::OpenButtonPressed())
    {
        EXIT_BUTTON_PRESSED = true;
        LOG_INFO("door's button input found");
    }
}

static void processStates()
{
    State lastState = state;

    if (HasState(GRANT_ACCESS))
    {
        grantAccess();
    }
    else if (HasState(DENY_ACCESS))
    {
        denyAccess();
    }
    else if (HasState(REQUESTING_AUTH))
    {
        if (HasState(VERIFY_PASSWORD))
            verifyPassword();
        else if (HasState(VERIFY_CARD))
            verifyCard();
    }
    else if (HasState(HANDLING_INPUT))
    {
        if (HasState(READING_PASSWORD))
            processPassword();
        else if (HasState(READING_CARD))
            processCard();
    }
    else if (IsWaiting())
    {
        wait();
    }

    if (EXIT_BUTTON_PRESSED)
        buttonPressed();

    stateChanged = state != lastState;
}

void loop()
{
    checkInputs();
    processStates();
}