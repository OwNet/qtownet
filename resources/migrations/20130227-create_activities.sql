CREATE TABLE activities (id INTEGER PRIMARY KEY AUTOINCREMENT,
user_name TEXT NOT NULL,
type INTEGER NOT NULL,
content  TEXT NOT NULL,
object_id INTEGER NOT NULL,
group_id INTEGER NOT NULL,
date_created TEXT,
date_updated TEXT
);
