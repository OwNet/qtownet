CREATE TABLE edges(page_id_from INTEGER NOT NULL,
page_id_to INTEGER NOT NULL,
date_created TEXT, 
date_updated TEXT,

PRIMARY KEY(page_id_from, page_id_to),

FOREIGN KEY(page_id_from) REFERENCES pages(id) ON DELETE CASCADE,
FOREIGN KEY(page_id_to) REFERENCES pages(id) ON DELETE CASCADE
);
