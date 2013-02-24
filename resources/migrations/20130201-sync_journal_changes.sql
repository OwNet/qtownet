DROP TABLE sync_journal;

CREATE TABLE "sync_journal" (
"id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"client_id" INTEGER NOT NULL,
"client_rec_num" INTEGER NOT NULL,
"table_name" TEXT NOT NULL,
"sync_id" TEXT NOT NULL,
"group_id" INTEGER,
"sync_with" INTEGER,
"date_created" TEXT NOT NULL,
"operation_type" INTEGER NOT NULL
);

DROP TABLE "client_caches";
DROP TABLE "group_admins";
DROP TABLE "group_types";
DROP TABLE "group_users";
DROP TABLE "groups";
DROP TABLE "messages";
DROP TABLE "ratings";
DROP TABLE "recommendations";
DROP TABLE "users";

CREATE TABLE "client_caches" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"client_id" INTEGER NOT NULL,
"cache_id" INTEGER NOT NULL,
"date_created" TEXT NOT NULL,
"sync_id" TEXT NOT NULL
);

CREATE TABLE "group_admins" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT,
"user_id" INTEGER NOT NULL,
"group_id" INTEGER NOT NULL,
"date_created" TEXT NOT NULL,
"date_updated" TEXT NOT NULL,
"sync_id" TEXT NOT NULL,
FOREIGN KEY ("user_id") REFERENCES "users"("id") ON DELETE CASCADE,
FOREIGN KEY ("group_id") REFERENCES "groups"("id") ON DELETE CASCADE
);


CREATE TABLE "group_types" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT,
"name" INTEGER NOT NULL,
"sync_id" TEXT NOT NULL
);


CREATE TABLE "group_users" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT,
"user_id" INTEGER NOT NULL,
"group_id" INTEGER NOT NULL,
"status" INTEGER NOT NULL,
"date_created" TEXT NOT NULL,
"date_updated" TEXT NOT NULL,
"sync_id" TEXT NOT NULL,
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
"sync_id" TEXT NOT NULL,
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
"sync_id" TEXT NOT NULL,
FOREIGN KEY ("user_id") REFERENCES "users"("id") ON DELETE CASCADE,
FOREIGN KEY ("parent_id") REFERENCES "messages"("id") ON DELETE CASCADE,
FOREIGN KEY ("group_id") REFERENCES "groups"("id") ON DELETE CASCADE
);


CREATE TABLE "ratings" (
"_id" INTEGER PRIMARY KEY AUTOINCREMENT,
"date_created" TEXT NOT NULL,
"user_id" INTEGER NOT NULL,
"absolute_uri" TEXT NOT NULL,
"val" INTEGER NOT NULL,
"sync_id" TEXT NOT NULL,
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
"sync_id" TEXT NOT NULL,
FOREIGN KEY ("user_id") REFERENCES "users"("id") ON DELETE SET NULL,
FOREIGN KEY ("group_id") REFERENCES "groups"("id") ON DELETE CASCADE
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
"sync_id" TEXT NOT NULL,
FOREIGN KEY ("role_id") REFERENCES "roles"("id")
);

