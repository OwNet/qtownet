DROP TABLE user_traverses_edges;
DROP TABLE user_visits_pages;

CREATE TABLE user_traverses_edges(user_id INTEGER,
edge_page_id_from INTEGER NOT NULL,
edge_page_id_to INTEGER NOT NULL,
frequency INTEGER NOT NULL,
date_created TEXT,
date_updated TEXT,

FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE,
FOREIGN KEY(edge_page_id_from, edge_page_id_to) REFERENCES edges(page_id_from, page_id_to) ON DELETE CASCADE,
PRIMARY KEY(user_id, edge_page_id_from, edge_page_id_to)
);

CREATE TABLE user_visits_pages(user_id INTEGER,
page_id INTEGER NOT NULL,
count INTEGER NOT NULL DEFAULT 1,
visited_at TEXT,
date_created TEXT,
date_updated TEXT,

FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE,
FOREIGN KEY(page_id) REFERENCES pages(id) ON DELETE CASCADE,
PRIMARY KEY(user_id, page_id)
);