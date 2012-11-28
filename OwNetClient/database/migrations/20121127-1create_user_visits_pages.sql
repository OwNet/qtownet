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
