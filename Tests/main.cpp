#include "autotest.h"

#include "syncservertests.h"
#include "proxyrequesttests.h"
#include "proxydownloadtests.h"

static Test<SyncServerTests> testSyncServerTests("SyncServerTests");
static Test<ProxyRequestTests> testProxyRequestTests("ProxyRequestTests");
static Test<ProxyDownloadTests> testProxyDownloadTests("ProxyDownloadTests");

TEST_MAIN
