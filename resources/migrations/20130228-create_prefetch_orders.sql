CREATE TABLE "prefetch_orders" (
"page_id_from" INTEGER,
"page_hash_to" INTEGER,
"absolute_uri" TEXT NOT NULL,
"completed" BOOL NOT NULL DEFAULT FALSE,
"priority" INTEGER NOT NULL DEFAULT 30,	
"date_created" TEXT,
"date_updated" TEXT,

PRIMARY KEY("page_id_from", "page_hash_to"),

FOREIGN KEY("page_id_from") REFERENCES "pages" ("id") ON DELETE CASCADE
);
