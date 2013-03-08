CREATE TABLE "activities" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT,
"user_name" TEXT NOT NULL,
"type" INTEGER NOT NULL,
"content" TEXT NOT NULL,
"object_id" TEXT NOT NULL,
"group_id" INTEGER NOT NULL,
"date_created" TEXT NOT NULL,
"date_updated" TEXT NOT NULL,
"uid" TEXT NOT NULL
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
CREATE TABLE "client_caches" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"client_id" TEXT NOT NULL,
"cache_id" INTEGER NOT NULL,
"date_created" TEXT NOT NULL,
"uid" TEXT NOT NULL
);
CREATE TABLE "client_sync_records" (
"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"client_id" TEXT NOT NULL,
"group_id" INTEGER,
"last_client_rec_num" INTEGER NOT NULL
);
CREATE TABLE edges(page_id_from INTEGER NOT NULL,
page_id_to INTEGER NOT NULL,
date_created TEXT, 
date_updated TEXT,

PRIMARY KEY(page_id_from, page_id_to),

FOREIGN KEY(page_id_from) REFERENCES pages(id) ON DELETE CASCADE,
FOREIGN KEY(page_id_to) REFERENCES pages(id) ON DELETE CASCADE
);
CREATE TABLE "group_admins" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT,
"user_id" INTEGER NOT NULL,
"group_id" INTEGER NOT NULL,
"date_created" TEXT NOT NULL,
"date_updated" TEXT NOT NULL,
"uid" TEXT NOT NULL,
FOREIGN KEY ("user_id") REFERENCES "users"("id") ON DELETE CASCADE,
FOREIGN KEY ("group_id") REFERENCES "groups"("id") ON DELETE CASCADE
);
CREATE TABLE "group_types" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT,
"name" INTEGER NOT NULL,
"uid" TEXT NOT NULL
);
CREATE TABLE "group_users" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT,
"user_id" INTEGER NOT NULL,
"group_id" INTEGER NOT NULL,
"status" INTEGER NOT NULL,
"date_created" TEXT NOT NULL,
"date_updated" TEXT NOT NULL,
"uid" TEXT NOT NULL,
FOREIGN KEY ("user_id") REFERENCES "users"("id") ON DELETE CASCADE,
FOREIGN KEY ("group_id") REFERENCES "groups"("id") ON DELETE CASCADE
);
CREATE TABLE "groups" (
"id" INTEGER PRIMARY KEY,
"name" TEXT NOT NULL,
"description" TEXT NOT NULL,
"password" TEXT,
"has_password" INTEGER NOT NULL,
"has_approvement" TEXT NOT NULL,
"group_type_id" INTEGER,
"parent" INTEGER,
"date_created" TEXT NOT NULL,
"date_updated" TEXT NOT NULL,
"salt" TEXT,
"uid" TEXT NOT NULL,
FOREIGN KEY ("parent") REFERENCES "groups"("id") ON DELETE CASCADE,
FOREIGN KEY ("group_type_id") REFERENCES "group_types"("id") ON DELETE SET NULL
);
CREATE TABLE "messages" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT,
"date_created" TEXT NOT NULL,
"user_id" INTEGER NOT NULL,
"group_id" INTEGER NOT NULL,
"message" TEXT NOT NULL,
"parent_id" INTEGER,
"uid" TEXT NOT NULL,
"date_updated" TEXT,
FOREIGN KEY ("user_id") REFERENCES "users"("id") ON DELETE CASCADE,
FOREIGN KEY ("parent_id") REFERENCES "messages"("id") ON DELETE CASCADE,
FOREIGN KEY ("group_id") REFERENCES "groups"("id") ON DELETE CASCADE
);
CREATE TABLE pages(id INTEGER PRIMARY KEY,
absolute_uri TEXT NOT NULL,
title TEXT NOT NULL,
date_created TEXT,
date_updated TEXT
);
CREATE TABLE "prefetch_orders" (
"page_id_from" INTEGER,
"page_hash_to" INTEGER PRIMARY KEY,
"absolute_uri" TEXT NOT NULL,
"completed" BOOL NOT NULL DEFAULT FALSE,
"priority" INTEGER NOT NULL DEFAULT 30,	
"date_created" TEXT,
"date_updated" TEXT,

FOREIGN KEY("page_id_from") REFERENCES "pages" ("id") ON DELETE CASCADE
);
CREATE TABLE "ratings" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT,
"date_created" TEXT NOT NULL,
"user_id" INTEGER NOT NULL,
"absolute_uri" TEXT NOT NULL,
"val" INTEGER NOT NULL,
"uid" TEXT NOT NULL,
FOREIGN KEY ("user_id") REFERENCES "users"("id") ON DELETE CASCADE
);
CREATE TABLE "recommendations" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT,
"absolute_uri" TEXT NOT NULL,
"title" TEXT NOT NULL,
"description" TEXT,
"user_id" INTEGER NOT NULL,
"group_id" INTEGER NOT NULL,
"date_created" TEXT,
"date_updated" TEXT,
"uid" TEXT NOT NULL,
FOREIGN KEY ("user_id") REFERENCES "users"("id") ON DELETE SET NULL,
FOREIGN KEY ("group_id") REFERENCES "groups"("id") ON DELETE CASCADE
);
CREATE TABLE roles(id INTEGER PRIMARY KEY AUTOINCREMENT,
name INTEGER NOT NULL
);
CREATE TABLE "settings" (
"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"key" TEXT NOT NULL UNIQUE,
"value" TEXT NOT NULL
);
CREATE TABLE "sync_journal" (
"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"client_id" TEXT NOT NULL,
"client_rec_num" INTEGER NOT NULL,
"table_name" TEXT NOT NULL,
"uid" TEXT NOT NULL,
"group_id" INTEGER,
"sync_with" INTEGER,
"date_created" TEXT NOT NULL,
"operation_type" INTEGER NOT NULL
);
CREATE TABLE user_traverses_edges(user_id INTEGER NOT NULL,
edge_page_id_from INTEGER NOT NULL,
edge_page_id_to INTEGER NOT NULL,
frequency INTEGER NOT NULL,
visited_at TEXT,
date_created TEXT,
date_updated TEXT,

FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE,
FOREIGN KEY(edge_page_id_from, edge_page_id_to) REFERENCES edges(page_id_from, page_id_to) ON DELETE CASCADE,
PRIMARY KEY(user_id, edge_page_id_from, edge_page_id_to)
);
CREATE TABLE user_visits_pages(user_id INTEGER NOT NULL,
page_id INTEGER NOT NULL,
count INTEGER NOT NULL DEFAULT 1,
visited_at TEXT,
date_created TEXT,
date_updated TEXT,

FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE,
FOREIGN KEY(page_id) REFERENCES pages(id) ON DELETE CASCADE,
PRIMARY KEY(user_id, page_id)
);
CREATE TABLE "users" (
"id" INTEGER PRIMARY KEY,
"first_name" TEXT NOT NULL,
"last_name" TEXT NOT NULL,
"login" TEXT NOT NULL,
"role_id" INTEGER NOT NULL,
"password" TEXT NOT NULL,
"email" TEXT,
"date_created" TEXT,
"date_updated" TEXT,
"salt" TEXT,
"uid" TEXT NOT NULL,
FOREIGN KEY ("role_id") REFERENCES "roles"("id")
);
CREATE INDEX "client_caches-cache_id" ON "client_caches" ("cache_id");
CREATE UNIQUE INDEX "client_sync_records_client_id_group_id" ON "client_sync_records" ("client_id", "group_id");
