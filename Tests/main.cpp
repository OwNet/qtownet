#include "autotest.h"

#include "syncservertests.h"
#include "proxydownloadtests.h"
#include "databaseupdatequerytests.h"
#include "workspaceinitializertests.h"
#include "workspacehelpertests.h"

static Test<SyncServerTests> testSyncServerTests("SyncServerTests");
static Test<ProxyDownloadTests> testProxyDownloadTests("ProxyDownloadTests");
static Test<DatabaseUpdateQueryTests> testDatabaseUpdateQueryTests("DatabaseUpdateQueryTests");
static Test<WorkspaceInitializerTests> testWorkspaceInitializerTests("WorkspaceInitializerTests");
static Test<WorkspaceHelperTests> testWorkspaceHelperTests("WorkspaceHelperTests");

TEST_MAIN
