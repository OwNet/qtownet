CREATE TABLE "client_caches" (
"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"client_id" INTEGER NOT NULL,
"cache_id" INTEGER NOT NULL,
"date_created" TEXT NOT NULL
);
CREATE UNIQUE INDEX "client_caches_client_id_cache_id_idx" ON "client_caches" ("client_id","cache_id");