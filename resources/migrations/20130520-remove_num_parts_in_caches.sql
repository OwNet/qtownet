DROP TABLE "caches";
CREATE TABLE "caches" (
"id" INTEGER PRIMARY KEY NOT NULL UNIQUE,
"absolute_uri" TEXT NOT NULL,
"date_created" TEXT NOT NULL,
"date_updated" TEXT NOT NULL,
"access_count" INTEGER NOT NULL DEFAULT 1,
"access_value" REAL NOT NULL DEFAULT 0.0,
"size" INTEGER NOT NULL DEFAULT 0
);
