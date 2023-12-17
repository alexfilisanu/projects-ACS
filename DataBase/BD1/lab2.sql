CREATE TABLE BIBLIOTECA(
	ID_biblioteca NUMBER(3) PRIMARY KEY,
	denumire VARCHAR2(100),
	localitate VARCHAR2(50),
	strada VARCHAR2(200)
);

CREATE TABLE CARTE(
	ID_carte NUMBER(5) PRIMARY KEY,
	titlu VARCHAR2(100),
	autor VARCHAR(100),
	nr_pagini NUMBER(4),
	data_aparitie DATE,
	ID_biblioteca NUMBER(3),
	CONSTRAINT FK_CARTE_BIBLIOTECA FOREIGN KEY (ID_biblioteca) REFERENCES BIBLIOTECA (ID_biblioteca)
);

INSERT INTO BIBLIOTECA (ID_biblioteca, denumire, localitate, strada) VALUES (1, 'Biblo1', 'BUCURESTI', 'Iuliu Maniu');
INSERT INTO BIBLIOTECA VALUES (2, 'Biblo2', 'Ploiesti', 'Unirii');
INSERT INTO BIBLIOTECA (ID_biblioteca, denumire) VALUES (3, 'Biblo3');

SELECT * FROM BIBLIOTECA; 

INSERT INTO CARTE VALUES (1, 'Carte buna', 'Autor bun', 500, TO_DATE('01-03-2000', 'DD-MM-YYYY'), 1);

SELECT ID_carte, titlu FROM CARTE;

DESC BIBLIOTECA;
DESC CARTE;

DROP TABLE CARTE;
DROP TABLE BIBLIOTECA; 
