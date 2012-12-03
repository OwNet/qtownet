CREATE TABLE "client_sync_records" (
"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"client_id" INTEGER NOT NULL,
"group_id" INTEGER,
"last_client_rec_num" INTEGER NOT NULL
);
CREATE UNIQUE INDEX "client_sync_records_client_id_group_id_idx" ON "client_sync_records" ("client_id","group_id");