CREATE TABLE recommendations(id INTEGER PRIMARY KEY AUTOINCREMENT,
absolute_uri TEXT NOT NULL,
title TEXT NOT NULL,
description TEXT,
user_id INTEGER NOT NULL,
group_id INTEGER NOT NULL,
date_created TEXT,
date_updated TEXT,
FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE SET NULL,
FOREIGN KEY(group_id) REFERENCES groups(id) ON DELETE CASCADE
);
