CREATE TABLE groups (id INTEGER PRIMARY KEY,
name TEXT NOT NULL,
description TEXT NOT NULL,
password TEXT,
has_password INTEGER NOT NULL,
has_approvement TEXT NOT NULL,
group_type_id INTEGER NOT NULL,
parent INTEGER NOT NULL,
date_created TEXT NOT NULL,
date_updated TEXT NOT NULL,

FOREIGN KEY(parent) REFERENCES groups(id) ON DELETE SET NULL,
FOREIGN KEY(group_type_id) REFERENCES roup_types(id) ON DELETE CASCADE
);
