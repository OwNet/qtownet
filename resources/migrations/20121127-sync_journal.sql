CREATE TABLE "sync_journal" (
"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"client_id" INTEGER NOT NULL,
"client_rec_num" INTEGER NOT NULL,
"content" TEXT NOT NULL,
"group_id" INTEGER,
"sync_with" INTEGER,
"date_created" TEXT NOT NULL
);
CREATE UNIQUE INDEX "sync_journal_client_client_rec_num_idx" ON "sync_journal" ("client_id","client_rec_num","group_id");