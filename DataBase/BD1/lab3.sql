CREATE TABLE DEPART(
	ID_DEP NUMBER(2) PRIMARY KEY,
	DEN_DEP VARCHAR2(10),
	NUME_SEF VARCHAR2(10),
	NR_ANG NUMBER(1)
);

CREATE TABLE ANGAJATI(
	ID_DEP NUMBER(2),
	ID_ANG NUMBER(1),
	NUME VARCHAR2(10),
	DATA_NASTERE DATE,
	SALARIU NUMBER(6,2),
	BONUS NUMBER(6,2),
	FUNCTIE VARCHAR2(10),
	CONSTRAINT PK_ANGAJATI PRIMARY KEY (ID_DEP, ID_ANG),
	CONSTRAINT FK_ANGAJATI_DEPART FOREIGN KEY (ID_DEP) REFERENCES DEPART (ID_DEP)
);

DESC DEPART
DESC ANGAJATI

INSERT INTO DEPART VALUES (1, 'DEPART1', 'SEFULUI1', 5);
INSERT INTO DEPART VALUES (2, 'DEPART2', 'SEFULUI2', 3);
INSERT INTO DEPART VALUES (3, 'DEPART3', 'SEFULUI3', 7);

INSERT INTO ANGAJATI VALUES (1, 1, 'DEP1ANG1', TO_DATE('05-03-1980', 'DD-MM-YYYY'), 4343.53, 1257.23, 'ITDEV');
INSERT INTO ANGAJATI VALUES (1, 2, 'DEP1ANG2', TO_DATE('05-03-1970', 'DD-MM-YYYY'), 5363.59, NULL, 'ITDEV');

INSERT INTO ANGAJATI VALUES (2, 1, 'DEP2ANG1', TO_DATE('05-07-1989', 'DD-MM-YYYY'), 3343.53, 256.23, 'ITDEV');
INSERT INTO ANGAJATI VALUES (2, 2, 'DEP2ANG2', TO_DATE('05-03-1960', 'DD-MM-YYYY'), 5363.59, 0, 'ITDEV');

INSERT INTO ANGAJATI VALUES (3, 1, 'DEP3ANG1', TO_DATE('05-07-1980', 'DD-MM-YYYY'), 4343.53, 5257.23, 'ITDEV');
INSERT INTO ANGAJATI VALUES (3, 2, 'DEP3ANG2', TO_DATE('25-05-1970', 'DD-MM-YYYY'), 5300.73, NULL, 'ITDEV');

SELECT * FROM DEPART;
SELECT * FROM ANGAJATI;

DEFINE DEP1 = 2;

SELECT NUME, SALARIU, NVL(BONUS, 0) BON 
	FROM ANGAJATI
	WHERE ID_DEP = &&DEP;

SELECT * FROM DEPART
	WHERE NUME_SEF != '&NUME' AND ID_DEP = &DEP1;

-- toti angajatii cu salariu > 200, functie = cea citita de la tastatura, nume != cel citit de la tastatura

SELECT * FROM ANGAJATI
	WHERE SALARIU > 200 
		AND FUNCTIE = '&FUNC'
		AND NUME != '&NUME';

-- ambele variante bonus egal cu 0

SELECT * FROM ANGAJATI
	WHERE NVL(BONUS, 0) = 0;

SELECT * FROM ANGAJATI
	WHERE BONUS = 0 OR BONUS IS NULL;

ACCEPT DEPAR NUMBER PROMPT 'Introduceti departamentul:'

SELECT * FROM DEPART
	WHERE ID_DEP = &DEPAR;

UNDEFINE DEP;
UNDEFINE DEP1;

DROP TABLE ANGAJATI;
DROP TABLE DEPART;
	