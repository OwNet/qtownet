CREATE TABLE "cache_clean_clocks" (
"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"value" REAL NOT NULL DEFAULT 0.0
);

CREATE TABLE "caches" (
"id" INTEGER PRIMARY KEY NOT NULL UNIQUE,
"absolute_uri" TEXT NOT NULL,
"request_headers" TEXT NOT NULL DEFAULT '',
"response_headers" TEXT NOT NULL DEFAULT '',
"date_created" TEXT NOT NULL,
"date_updated" TEXT NOT NULL,
"num_parts" INTEGER DEFAULT 0 NOT NULL,
"status_code" INTEGER NOT NULL DEFAULT 200,
"status_description" TEXT DEFAULT 'OK',
"access_count" INTEGER NOT NULL DEFAULT 1,
"access_value" REAL NOT NULL DEFAULT 0.0,
"size" INTEGER NOT NULL DEFAULT 0
);
