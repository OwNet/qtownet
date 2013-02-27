CREATE TABLE users (id INTEGER PRIMARY KEY,
first_name TEXT NOT NULL,
last_name TEXT NOT NULL,
login TEXT NOT NULL,
role_id INTEGER NOT NULL,
password TEXT NOT NULL,
email TEXT,
date_created TEXT,
date_updated TEXT,
FOREIGN KEY(role_id) REFERENCES roles(id)
);