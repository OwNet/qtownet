#include "autotest.h"

#include "syncservertests.h"
#include "proxydownloadtests.h"
#include "databaseupdatequerytests.h"
#include "workspaceinitializertests.h"
#include "workspacehelpertests.h"
#include "databaseselectquerytests.h"
#include "proxycachelocationstests.h"

static Test<SyncServerTests> testSyncServerTests("SyncServerTests");
static Test<ProxyDownloadTests> testProxyDownloadTests("ProxyDownloadTests");
static Test<DatabaseUpdateQueryTests> testDatabaseUpdateQueryTests("DatabaseUpdateQueryTests");
static Test<WorkspaceInitializerTests> testWorkspaceInitializerTests("WorkspaceInitializerTests");
static Test<WorkspaceHelperTests> testWorkspaceHelperTests("WorkspaceHelperTests");
static Test<DatabaseSelectQueryTests> testDatabaseSelectQueryTests("DatabaseSelectQueryTests");
static Test<ProxyCacheLocationsTests> testProxyCacheLocationsTests("ProxyCacheLocationsTests");

TEST_MAIN
