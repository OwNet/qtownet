CREATE TABLE "shared_files" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"uid" TEXT NOT NULL,
"date_created" TEXT NOT NULL,
"date_updated" TEXT NOT NULL,
"url" TEXT NOT NULL,
"content_type" TEXT NOT NULL,
"title" TEXT,
"description" TEXT,
"cache_id" INTEGER NOT NULL
);
