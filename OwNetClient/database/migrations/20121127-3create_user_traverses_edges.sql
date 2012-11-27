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
