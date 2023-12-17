/*
sa se scrie o cerere sql care face o lista cu toti angajatii departamentului SALES
in care sa se afiseze drepturile salariale si primele angajatilor

daca grad salarial < 3 => prima = sal maxim din firma
daca grad salarial = 3 => prima = sal mediu
daca grad salaraial >= 4 => prima = sal minim

antetul listei: numele angajatului, denumire depart, grad salarial, si prima calculata

2 metode 
*/

SELECT A.ENAME, D.DNAME, S.GRADE, 
	DECODE(SIGN(S.GRADE - 3), -1, MAX(C.SAL), 0, AVG(C.SAL), MIN(C.SAL)) PRIMA	
	FROM EMP A, EMP C, DEPT D, SALGRADE S
	WHERE A.DEPTNO = D.DEPTNO
		AND A.SAL BETWEEN S.LOSAL AND S.HISAL
		AND D.DNAME = 'SALES'
	GROUP BY A.ENAME, D.DNAME, S.GRADE;

SELECT A.ENAME, D.DNAME, S.GRADE,
	CASE
	  WHEN(S.GRADE < 3) THEN MAX(C.SAL)
	  WHEN(S.GRADE = 3) THEN AVG(C.SAL)
	  ELSE MIN(C.SAL)
	END PRIMA 
	FROM EMP A NATURAL JOIN DEPT D, EMP C, SALGRADE S
	WHERE A.SAL BETWEEN S.LOSAL AND S.HISAL
		AND D.DNAME LIKE 'SALES'
	GROUP BY A.ENAME, D.DNAME, S.GRADE;
	