-- pentru fiecare angajat sa se afiseze numele lui, denumirea depart, venitul si o apeciere a venitului
-- apreciere: venit < 1500 => slabut, altfel ok

-- DECODE(EXPR, VAL2, REZ1,
--		VAL2, REZ2, ...
-- 		REZD)

SELECT A.ENAME, B.DNAME, A.SAL + NVL(A.COMM, 0) VENIT, 
	DECODE(SIGN(A.SAL + NVL(A.COMM, 0) - 1500 ), -1, 'SLABUT', 'OK') APRECIERE
	FROM EMP A JOIN DEPT B ON A.DEPTNO = B.DEPTNO
	WHERE NVL(A.COMM, 0) = 0; 

-- pentru fiecare angajat sa se afiseze numele lui, denumirea depart, venitul si o apeciere a venitului
-- apreciere: venit < 1500 => slabut, 1500 <= venit < 2000 ok, altfel bun

SELECT A.ENAME, B.DNAME, A.SAL + NVL(A.COMM, 0) VENIT, 
	DECODE(SIGN(A.SAL + NVL(A.COMM, 0) - 1500), -1, 'SLABUT', 
		DECODE(SIGN(A.SAL + NVL(A.COMM, 0) - 2000), -1, 'OK', 'BUN')) APRECIERE
	FROM EMP A JOIN DEPT B ON A.DEPTNO = B.DEPTNO
	WHERE NVL(A.COMM, 0) = 0; 

-- pt fiecare angajat sa se afiseze numele lui, gradul salarial, job ul, o traducere in limba romana a jobului

SELECT A.ENAME, S.GRADE, A.JOB, DECODE(A.JOB, 'CLERK', 'FUNCTIONAR', 
		'SALESMAN', 'AGENT VANZARI', 'MANAGER', 'MANAGER', 'ANALYST', 'ANALIST',
		'PRESIDENT', 'SEF') TRADUCERE
	FROM EMP A, SALGRADE S
	WHERE A.SAL BETWEEN S.LOSAL AND S.HISAL;

-- CASE EXPR
--   WHEN VAL1 THEN REZ1
--   WHEN VAL2 THEN REZ2
--   ...
--   ELSE RESD
-- END 

SELECT A.ENAME, S.GRADE, A.JOB, 
	CASE A.JOB
	  WHEN 'CLERK' THEN 'FUNCTIONAR'
	  WHEN 'SALESMAN' THEN 'AGENT VANZARI'
	  WHEN 'MANAGER' THEN 'MANAGER'
	  WHEN 'ANALYST' THEN 'ANALIST'
	  WHEN 'PRESIDENT' THEN 'SEF'
	END TRADUCERE
	FROM EMP A, SALGRADE S
	WHERE A.SAL BETWEEN S.LOSAL AND S.HISAL;

-- CASE
--   WHEN EXPR1 THEN REZ1
--   WHEN EXPR2 THEN REZ2
--   ...
--   ELSE RESD
-- END

SELECT A.ENAME, B.DNAME,A.SAL + NVL(A.COMM, 0) VENIT,
	CASE
	  WHEN(A.SAL + NVL(A.COMM, 0) < 1500) THEN 'SLABUT'
	  WHEN(A.SAL + NVl(A.COMM, 0) <= 2000) THEN 'OK'
	  ELSE 'BUN'
	END
	FROM EMP A JOIN DEPT B ON A.DEPTNO = B.DEPTNO
	WHERE NVL(A.COMM, 0) = 0; 

-- pt toti angajatii depart research -> o lista care sa contina aprecierea vechimii salariale
-- apreciere: au venit inainte de 1 ian 81 -> 'FOARTE VECHI', intre 81-86 -> 'VECHI', > 86 -> 'RECENT'

SELECT A.ENAME, A.HIREDATE,
	CASE
	  WHEN(A.HIREDATE < TO_DATE('01-01-1981', 'DD-MM-YYYY')) THEN 'FOARTE VECHI'
	  WHEN(A.HIREDATE < TO_DATE('01-01-1987', 'DD-MM-YYYY')) THEN 'VECHI'
	  ELSE 'RECENT'
	END VECHIME
	FROM EMP A, DEPT D
	WHERE A.DEPTNO = D.DEPTNO
		AND D.DNAME = 'RESEARCH'; 

-- MAX, MIN, AVG, SUM, COUNT

SELECT MAX(SAL)
	FROM EMP A JOIN DEPT D ON A.DEPTNO = D.DEPTNO
	WHERE D.DNAME = 'SALES';

-- GROUP BY - HAVING(echivalent WHERE pt SELECT)

-- afiseaza nume dep, cati angajati are si sal maxim, numar angajati >= 4 

SELECT D.DNAME, MAX(A.SAL), COUNT(A.EMPNO)
	FROM EMP A JOIN DEPT D ON A.DEPTNO = D.DEPTNO
	GROUP BY D.DNAME
	HAVING COUNT(*) >= 4;

-- pt fiecare grad salarial sa se afiseze gradul si nr angajatilor care au salariul in acel
-- grad si care nu au comision; gradele care au mai mult de 1 angajat

SELECT S.GRADE, COUNT(A.EMPNO)
	FROM EMP A JOIN SALGRADE S ON A.SAL BETWEEN S.LOSAL AND S.HISAL
	WHERE NVL(COMM, 0) = 0
	GROUP BY S.GRADE
	HAVING COUNT(*) >= 1;

-- sa se realizeze o lista de premierea a angajatilor astfel: angajatii care au primit comision
-- primesc o prima = sal mediu pe companie, care nu au primit comision primesc o prima = sal minim pe economie
-- presedintele si managerii nu primesc prima, sal si prima se afiseaza fara zecimale

SELECT A.ENAME, A.COMM, A.JOB, AVG(C.SAL), MIN(C.SAL), 
	TRUNC(DECODE(A.JOB, 'MANAGER', 0, 'PRESIDENT', 0, 
		DECODE(NVL(A.COMM, 0), 0, MIN(C.SAL), AVG(C.SAL)))) PRIMA	
	FROM EMP A, EMP C
	GROUP BY A.ENAME, A.COMM, A.JOB;
