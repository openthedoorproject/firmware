#pragma once

namespace Door
{
    void Init(int doorLockPort, int openButtonPort);

    void SetHighOnClose(bool state);

    void SetOpenTimeout(long millisseconds);

    bool OpenButtonPressed();

    bool IsOpened();

    bool GrantAccess();

    void ResetTimer();
}