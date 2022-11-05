#include "System.h"

#include "Keyboard/Keyboard.h"

#include "Door/Door.h"
#include "Display/Display.h"
#include "Card/CardReader.h"

#include "Auth/PasswordParser.h"
#include "Auth/Auth.h"

#include <LiquidCrystal_I2C.h>

enum State : int
{
    WAITING = 0,

    READING_PASSWORD = 1,
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

    GRANT_ACCESS = VERIFY_CARD << 1,

    MALFORMED_PASSWORD = GRANT_ACCESS << 1
};

State state;

inline bool HasState(State search)
{
    return ((int)state) & ((int)search);
}

inline void ToggleState(State targetState)
{
    ::state = static_cast<State>(((int)::state) ^ ((int)targetState));
}

inline void AddState(State state)
{
    ::state = static_cast<State>(((int)::state) | ((int)state));
}

inline void RemoveState(State state)
{
    ::state = static_cast<State>(((int)::state) & (~((int)state)));
}

inline void SetState(State state)
{
    ::state = state;
}

void setup()
{
    LOG_INIT(9600);
    Door::Init(4, 13);
    Door::SetOpenTimeout(7000);

    Keyboard::Init(4, 4, new int[4]{12, 14, 27, 26}, new int[4]{25, 33, 32, 5});

    Keyboard::SetRow(0, new char[4]{'1', '2', '3', 'A'});
    Keyboard::SetRow(1, new char[4]{'4', '5', '6', 'B'});
    Keyboard::SetRow(2, new char[4]{'7', '8', '9', 'C'});
    Keyboard::SetRow(3, new char[4]{'*', '0', '#', 'D'});

    PasswordParser::SetTimeout(4000);
    PasswordParser::SetPasswordSize(4);

    CardReader::Init(22, 21);

    state = WAITING;

    LOG_INFO("setup finished");
}

static void processPassword()
{
    PasswordParser::ParsingState parsingState = PasswordParser::Parse();

    if (parsingState & PasswordParser::ENDED)
    {
        if (parsingState == PasswordParser::ERROR)
        {
            LOG_WARN("error while parsing password");
            AddState(LED_RED);
        }
        else
        {
            LOG_INFO("parsing password finished");
            AddState(VERIFY_PASSWORD);
        }
        RemoveState(READING_PASSWORD);
    }
}

static void verifyPassword()
{
    String password = PasswordParser::GetParsedPassword();

    if (Auth::VerifyPassword(password))
    {
        LOG_INFO("password accepted");
        AddState(GRANT_ACCESS);
    }
    else
    {
        LOG_INFO("invalid password");
        AddState(LED_RED);
    }

    RemoveState(VERIFY_PASSWORD);
}

static void processCard()
{
    if (CardReader::ReadCard() == CardReader::ERROR)
    {
        LOG_ERROR("fail to read card");
        AddState(LED_RED);
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

    if (Auth::VerifyCard(inputUID.HexString()))
    {
        AddState(GRANT_ACCESS);
    }
    else
    {
        LOG_INFO("invalid card");
        AddState(LED_RED);
    }

    RemoveState(VERIFY_CARD);
}

static void grantAccess()
{
    if (!Door::GrantAccess())
        RemoveState(GRANT_ACCESS);
}

static void checkInputs()
{
    if (!HasState(HANDLING_INPUT))
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
        AddState(GRANT_ACCESS);
        LOG_INFO("door's button input found");
    }
}

static void processStates()
{
    if (HasState(GRANT_ACCESS))
    {
        grantAccess();
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
}

void loop()
{
    checkInputs();
    processStates();
}