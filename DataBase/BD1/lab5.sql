SELECT ENAME, SAL + NVL(COMM, 0) VENIT
	FROM EMP A, DEPT B
	WHERE A.DEPTNO = B.DEPTNO
		AND (COMM IS NULL OR COMM = 0)
		AND B.DNAME = 'ACCOUNTING';

-- acelasi ca cel de sus, dar sintaxa de JOIN
-- JOIN ON == JOIN USING

SELECT ENAME, SAL + NVL(COMM, 0) VENIT
	FROM EMP A JOIN DEPT B ON A.DEPTNO = B.DEPTNO
	WHERE (COMM IS NULL OR COMM = 0)
		AND B.DNAME = 'ACCOUNTING';

-- caz particular DEPTNO ac denumire -> NATURAL JOIN

SELECT ENAME, SAL + NVL(COMM, 0) VENIT
	FROM EMP A NATURAL JOIN DEPT B
	WHERE (COMM IS NULL OR COMM = 0)
		AND B.DNAME = 'ACCOUNTING';

-- sa se selecteze toti angajatii care nu lucreaza in chicago, care fac parte dintr un depart citit de la tastatura
-- afisand numele, denumirea depart, localitatea

ACCEPT DEPATTASTATURA CHAR PROMPT 'Introduceti numele departamentului:'

SELECT ENAME, DNAME, LOC
	FROM EMP, DEPT
	WHERE EMP.DEPTNO = DEPT.DEPTNO
		AND LOC != 'CHICAGO'
		AND DNAME = '&DEPATTASTATURA';

SELECT ENAME, DNAME, LOC
	FROM EMP JOIN DEPT ON EMP.DEPTNO = DEPT.DEPTNO
	WHERE LOC != 'CHICAGO'
		AND DNAME = '&DEPATTASTATURA';

SELECT ENAME, DNAME, LOC
	FROM EMP JOIN DEPT USING (DEPTNO)
	WHERE LOC != 'CHICAGO'
		AND DNAME = '&DEPATTASTATURA';

SELECT ENAME, DNAME, LOC
	FROM EMP NATURAL JOIN DEPT
	WHERE LOC != 'CHICAGO'
		AND DNAME = '&DEPATTASTATURA';

UNDEFINE DEPATTASTATURA;

-- selecteaza toti angajatii care il au ca sef pe KING
-- afiseaza numele angajatului, venitul, numele sefului

SELECT A.ENAME, A.SAL + NVL(A.COMM, 0) VENIT, SEF.ENAME
	FROM EMP A, EMP SEF
	WHERE A.MGR = SEF.EMPNO
		AND SEF.ENAME = 'KING';

SELECT A.ENAME, A.SAL + NVL(A.COMM, 0) VENIT, SEF.ENAME
	FROM EMP A JOIN EMP SEF ON A.MGR = SEF.EMPNO
	WHERE SEF.ENAME = 'KING';

-- cerinta anterioara + castiga > BLAKE

SELECT A.ENAME, A.SAL + NVL(A.COMM, 0) VENIT, SEF.ENAME, BLAKE.SAL + NVL(BLAKE.COMM, 0) VENIT_BLAKE
	FROM EMP A, EMP SEF, EMP BLAKE
	WHERE A.MGR = SEF.EMPNO
		AND SEF.ENAME = 'KING'
		AND BLAKE.ENAME = 'BLAKE'
		AND A.SAL + NVL(A.COMM, 0) > BLAKE.SAL + NVL(BLAKE.COMM, 0);

-- sa se selecteze toti angajatii care castiga > ADAMS
-- afisand numele angajatului, numele sefului angajatului si venitul aSngajatului

SELECT A.ENAME, SEF.ENAME NUME_SEF, A.SAL + NVL(A.COMM, 0) VENIT
	FROM EMP A, EMP SEF, EMP ADAMS
	WHERE A.MGR = SEF.EMPNO
		AND ADAMS.ENAME = 'ADAMS'
		AND A.SAL + NVL(A.COMM, 0) > ADAMS.SAL + NVL(ADAMS.COMM, 0);

--outer join (+) la tabela mai saraca / RIGHT sau LEFT la cea mai bogata

SELECT DNAME, ENAME
	FROM EMP, DEPT
	WHERE EMP.DEPTNO (+)= DEPT.DEPTNO;

SELECT DNAME, ENAME
	FROM EMP A RIGHT OUTER JOIN DEPT B
		ON A.DEPTNO = B.DEPTNO;

-- angajati cu venit > sefii lor si care au venit in firma dupa 1 ian 82

SELECT A.ENAME, A.SAL + NVL(A.COMM, 0) VENIT_ANG, A.HIREDATE,
       B.ENAME, B.SAL + NVL(B.COMM, 0) VENIT_SEF
	FROM EMP A JOIN EMP B ON A.MGR = B.EMPNO
	WHERE A.SAL + NVL(A.COMM, 0) > B.SAL + NVL(B.COMM, 0)
		AND A.HIREDATE > TO_DATE('01-01-1982', 'DD-MM-YYYY');
		-- TO_CHAR(A.HIREDATE, 'YYYYMMDD') > '19820101'

-- join cu salgrade

SELECT A.ENAME, A.SAL, B.GRADE
	FROM EMP A, SALGRADE B
	WHERE A.SAL >= B.LOSAL
		AND A.SAL <= B.HISAL;

SELECT A.ENAME, A.SAL, B.GRADE
	FROM EMP A JOIN SALGRADE B ON A.SAL >= B.LOSAL
		AND A.SAL <= B.HISAL;

SELECT A.ENAME, A.SAL, B.GRADE
	FROM EMP A, SALGRADE B
	WHERE A.SAL BETWEEN B.LOSAL AND B.HISAL;

SELECT A.ENAME, A.SAL, B.GRADE
	FROM EMP A JOIN SALGRADE B ON A.SAL BETWEEN B.LOSAL AND B.HISAL;
