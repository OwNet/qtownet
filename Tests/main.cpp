#include "autotest.h"

#include "syncservertests.h"
#include "proxydownloadtests.h"
#include "databaseupdatequerytests.h"

static Test<SyncServerTests> testSyncServerTests("SyncServerTests");
static Test<ProxyDownloadTests> testProxyDownloadTests("ProxyDownloadTests");
static Test<DatabaseUpdateQueryTests> testDatabaseUpdateQueryTests("DatabaseUpdateQueryTests");

TEST_MAIN
