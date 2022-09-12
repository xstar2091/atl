#pragma once


namespace atl {

class TcpTask;

class Task {
public:
    virtual ~Task();

public:
    TcpTask* CreateTcpTask();
};

}
