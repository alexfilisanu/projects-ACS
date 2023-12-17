/*
	1. NUM
	- FLOOR(n)
	- CEIL(n)
	- ROUND(a, b) ROUND(15.1234, 2) = 15.12 ROUND(15.1234, -1) = 20
	- TRUNC(a, b)
	- MOD(a, b)

*/

-- Sa se selecteze toti angajatii care au id ul impar, selectand numele, id ul, salariul impartit la 3, intregii de salariu impartit la 3,
-- valoarea rotunjita din sutimi din salariu impartit la 3 si apoi valoarea rotunjita la sute din salariul impartit la 3

SELECT ENAME, EMPNO, SAL / 3, TRUNC(SAL / 3, 2), ROUND(SAL / 3, 2), ROUND(SAL / 3, -2)  
	FROM EMP
	WHERE (MOD(EMPNO, 2) = 1);

-- Creati o lista in care sa fie calculata o prima pentru angajatii care nu primesc comision, nu au functia
-- de manager si s au angajat inainte de ALLEN, prima = 23% din venit angajatului, rotunjita la intregi
-- afisand numele, functia, salariul, data angajarii, data angajarii lui Allen, prima

SELECT A.ENAME, A.JOB, A.SAL, A.HIREDATE, ALLEN.HIREDATE ALLEN_HIREDATE, ROUND(A.SAL * 0.23, 0) PRIMA 
	FROM EMP A, EMP ALLEN
	WHERE ALLEN.ENAME = 'ALLEN'
		AND A.HIREDATE < ALLEN.HIREDATE
		AND A.JOB != 'MANAGER'
		AND NVL(A.COMM, 0) = 0;

SELECT A.ENAME, A.JOB, A.SAL, A.HIREDATE, ALLEN.HIREDATE ALLEN_HIREDATE, ROUND(A.SAL * 0.23, 0) PRIMA 
	FROM EMP A JOIN EMP ALLEN ON ALLEN.ENAME = 'ALLEN'
	WHERE A.HIREDATE < ALLEN.HIREDATE
		AND A.JOB != 'MANAGER'
		AND NVL(A.COMM, 0) = 0;

/* 
	2. SIR
	- CONCAT(a, b) ~ a || b
	- TRANSLATE(SIR_MARE, SIR1, SIR2) 
	- REPLACE(SIR_MARE, SIR1, SIR2)

ex: SIR_MARE = 'FACULTATE'
    REPLACE('FACULTATE', 'TA', 'XY') = 'FACULXYTE'
    TRANSLATE('FACULTATE', 'TA', 'XY') = 'FYCULXYXE'
    TRANSLATE('FACULTATE', 'TA', 'X') = 'FCULXXE'
	
	- LENGTH(a)
	- SUBSTR
*/

-- selecteaza toti angajatii carer nu au comision, SAL > 1000, numele, localitatea unde lucreaza,
-- numele lui unde subsirul 'AR' se inlocuieste cu 'XY', numele lui unde subsirul 'AR' se
-- translateaza cu 'X', lungimea numelui angajatului

SELECT A.ENAME, DEP.LOC, REPLACE(A.ENAME, 'AR', 'XY'), TRANSLATE(A.ENAME, 'AR', 'X'), LENGTH(A.ENAME)
	FROM EMP A, DEPT DEP
	WHERE A.DEPTNO = DEP.DEPTNO
		AND A.SAL > 1000
		AND NVL(A.COMM, 0) = 0;

-- sa se selecteze numarul de aparitii ale ulimelor 2 litere din numele angajatului
-- care se gasesc in jobul angajatului exact in ordinea respectiva, afiseaza numele, nr aparitii

SELECT ENAME, SUBSTR(ENAME, -2) ULTIMELE, REPLACE(JOB, SUBSTR(ENAME, -2), '') SIR_MIC, 
		(LENGTH(JOB) - LENGTH(REPLACE(JOB, SUBSTR(ENAME, -2), ''))) / 2
	FROM EMP
	WHERE (LENGTH(JOB) - LENGTH(REPLACE(JOB, SUBSTR(ENAME, -2), ''))) / 2 > 0;

-- selectati toti angajatii din departamentele cu denumirea diferite de o valoare citita de la tastatura,
-- angajati care contin numele lor litera C si care nu primesc comision. Numele angajatului se concateneaza
-- cu denumirea departamentului sub un sir de forma 'angajatul ename este intr un depart dname' si se va
-- afisa si venitul angajatului cu valori rotunjite la zeci

ACCEPT DEPARTNAME CHAR PROMPT 'Introduceti numele departamentului:'

SELECT 'angajatul ' || A.ENAME || ' este intr un depart ' || DEP.DNAME, ROUND((A.SAL + NVL(A.COMM, 0)), -1)
	FROM EMP A JOIN DEPT DEP USING(DEPTNO)
	WHERE DEP.DNAME != '&DEPARTNAME'
		AND LENGTH(REPLACE(A.ENAME, 'C', '')) != LENGTH(A.ENAME)
		AND NVL(A.COMM, 0) = 0;

SELECT CONCAT(CONCAT(CONCAT('angajatul ', A.ENAME), ' este intr un depart '), DEP.DNAME), ROUND((A.SAL + NVL(A.COMM, 0)), -1)
	FROM EMP A, DEPT DEP
	WHERE A.DEPTNO = DEP.DEPTNO
		AND DEP.DNAME != '&DEPARTNAME'
		AND A.ENAME LIKE '%C%'
		AND NVL(A.COMM, 0) = 0;

/* 
	SYSDATE - data curenta
	SYS.DUAL - tabela cu o singura inregistrare
*/

SELECT SYSDATE FROM SYS.DUAL;

/*
	3. DATA
	- LAST_DAY(D)
	- NEXT_DAY(D, 'MONDAY')
	D1 - D2 = nr de zile
	D1 + n = D2 
*/

-- extragere de la fiecare angajat anul, luna, data, separate
-- EXTRACT DAY FROM HIREDATE ~ TO_CHAR(HIREDATE, 'DD')

SELECT ENAME, EXTRACT(DAY FROM HIREDATE) ZI, EXTRACT(MONTH FROM HIREDATE) LUNA, EXTRACT(YEAR FROM HIREDATE) AN
	FROM EMP;

-- sa se selecteze urmatoarea zi de joi si ultima zi din luna curenta

SELECT NEXT_DAY(SYSDATE, 'THURSDAY') URM_JOI, LAST_DAY(SYSDATE) ULTIMA
	FROM SYS.DUAL;

-- lista cu data testarii celor din depart sales; testarea va avea loc dupa 2 luni de la angajare, in ultima zi din sapt respectiva

SELECT A.ENAME, A.HIREDATE, NEXT_DAY(ADD_MONTHS(A.HIREDATE, 2), 'SUNDAY'), DEP.DNAME
	FROM EMP A, DEPT DEP
	WHERE A.DEPTNO = DEP.DEPTNO
		AND DEP.DNAME = 'SALES';

-- padding la stanga cu X
SELECT ENAME, LPAD(SAL, 6, 'X')
	FROM EMP;

-- DISTINCT - fara duplicate

SELECT DISTINCT A.MGR, B.ENAME
	FROM EMP A JOIN EMP B ON A.MGR = B.EMPNO
	ORDER BY MGR;

/*
	4. MULTIMI
	- UNION
	- INTERSECT
*/

-- pentru fiecare angajat sa se afiseze numele lui, denumirea depart, venitul si o apeciere a venitului
-- apreciere: venit < 2000 => scazut, altfel bun

SELECT A.ENAME, B.DNAME, A.SAL + NVL(A.COMM, 0) VENIT, 'SCAZUT'
	FROM EMP A JOIN DEPT B ON A.DEPTNO = B.DEPTNO
	WHERE A.SAL + NVL(A.COMM, 0) < 2000 
UNION
SELECT A.ENAME, B.DNAME, A.SAL + NVL(A.COMM, 0) VENIT, 'BUN'
	FROM EMP A JOIN DEPT B ON A.DEPTNO = B.DEPTNO
	WHERE A.SAL + NVL(A.COMM, 0) >= 2000
ORDER BY 1;

-- selectati numele sefilor tuturor angajatilor care nu fac parte dintr un departament citit 
-- de la tastatura si care au un venit mai mare decat BLAKE;
-- veti mai afisa o val medie dintre salariu si comision pt fiecare angajat, rotujita;
-- nu afisati duplicate

SELECT DISTINCT SEF.ENAME, ROUND((A.SAL + NVL(A.COMM, 0)) / 2)
	FROM EMP A, DEPT DEP, EMP BLAKE, EMP SEF  
	WHERE A.DEPTNO = DEP.DEPTNO
		AND BLAKE.ENAME = 'BLAKE'
		AND SEF.EMPNO = A.MGR
		AND A.SAL + NVL(A.COMM, 0) > BLAKE.SAL + NVL(BLAKE.COMM, 0)			
		AND DEP.DNAME != '&DEPARTNAME'
	ORDER BY 1;

SELECT DISTINCT SEF.ENAME, ROUND((A.SAL + NVL(A.COMM, 0)) / 2)
	FROM EMP A NATURAL JOIN DEPT DEP, EMP BLAKE, EMP SEF  
	WHERE BLAKE.ENAME = 'BLAKE'
		AND SEF.EMPNO = A.MGR
		AND BLAKE.SAL + NVL(BLAKE.COMM, 0) BETWEEN 0 AND A.SAL + NVL(A.COMM, 0) 		
		AND DEP.DNAME NOT LIKE '&DEPARTNAME'
	ORDER BY 1;

UNDEFINE DEPARTNAME;
