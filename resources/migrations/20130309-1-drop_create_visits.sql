DROP TABLE user_visits_pages;

CREATE TABLE user_visits_pages(id INTEGER PRIMARY KEY AUTOINCREMENT,
user_id INTEGER,
page_id INTEGER NOT NULL,
date_created TEXT,
date_updated TEXT,

FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE,
FOREIGN KEY(page_id) REFERENCES pages(id) ON DELETE CASCADE
);