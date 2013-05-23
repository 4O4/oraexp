#ifndef CONNECTIONPAGEOBJECT_H
#define CONNECTIONPAGEOBJECT_H

#include "util/queryscheduler.h"

class DbConnection;

class ConnectionPageObject : public QueryScheduler
{
public:
    ConnectionPageObject();
    virtual ~ConnectionPageObject();

    //this function can be used to create user interface if tab wants
    //to allow inherited classes to override ui creation logic
    virtual void createUi(){}

    bool needsSeparateConnection(){return  requiresSeparateConnection;}
    virtual void setConnection(DbConnection *db);

    virtual DbConnection *getDb() const {return this->db;}
    virtual void increaseRefCount();
    virtual void decreaseRefCount();

    bool isBusy() const;

protected:
    DbConnection *db;
    bool requiresSeparateConnection;
    bool busy;

    int activeQueryCount;

    void beforeEnqueueQuery();

    void setBusy(bool busy);

    virtual void queryExecTaskCompleted(const QString &taskName) = 0;
    virtual void emitBusyStateChangedSignal() = 0;
    virtual void emitInitCompletedSignal() = 0;
};

#endif // CONNECTIONPAGEOBJECT_H