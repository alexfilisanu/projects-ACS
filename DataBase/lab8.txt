-- pentru toti angajatii din departamentul SALES sa se afiseze numele, 
-- numele sefului si gradul salarial

SELECT A.ENAME, SEF.ENAME, S.GRADE
	FROM EMP A 
		JOIN EMP SEF ON A.MGR = SEF.EMPNO 
		JOIN SALGRADE S ON A.SAL BETWEEN S.LOSAL AND S.HISAL
		JOIN DEPT D ON A.DEPTNO = D.DEPTNO
	WHERE DNAME = 'SALES';

SELECT A.ENAME, S.ENAME, C.GRADE
	FROM EMP A 
		JOIN EMP S ON A.MGR = S.EMPNO
		JOIN SALGRADE C ON A.SAL BETWEEN C.LOSAL AND C.HISAL
	WHERE A.DEPTNO = (SELECT DEPTNO FROM DEPT WHERE DNAME = 'SALES');

-- sa se selecteze toti angajatii care castiga mai mult decat salariul mediu din depart accounting

SELECT A.ENAME, A.SAL
	FROM EMP A
	WHERE A.SAL > (SELECT AVG(B.SAL)
				FROM EMP B JOIN DEPT D ON B.DEPTNO = D.DEPTNO
				WHERE D.DNAME = 'ACCOUNTING');

SELECT A.ENAME, A.SAL 
	FROM EMP A 
	WHERE A.SAL > (SELECT AVG(B.SAL) 
				FROM EMP B 
                WHERE B.DEPTNO = (SELECT DEPTNO 
								FROM DEPT 
								WHERE DNAME = 'ACCOUNTING'));

-- sa se selecteze angajatul care face parte din departamentul 'sales' si castiga cel mai mult

SELECT A.ENAME, A.SAL + NVL(A.COMM, 0) VENIT
	FROM EMP A
	WHERE A.DEPTNO = (SELECT B.DEPTNO 
				FROM DEPT B 
				WHERE B.DNAME = 'SALES')
		AND A.SAL + NVL(A.COMM, 0) = (SELECT MAX(C.SAL + NVL(C.COMM, 0)) 
									FROM EMP C
									WHERE C.DEPTNO = (SELECT DEPTNO 
													FROM DEPT 
													WHERE DNAME = 'SALES'));

-- sa se selecteze toti angjatii care care castiga mai mult decat media venitului din departamentul lor

SELECT A.ENAME, A.SAL + NVL(A.COMM, 0) VENIT
	FROM EMP A 
	WHERE A.SAL + NVL(A.COMM, 0) > (SELECT AVG(B.SAL + NVL(B.COMM, 0))
										FROM EMP B
										WHERE B.DEPTNO = A.DEPTNO);

-- pt toti angajatii care castiga mai mult decat seful lor si care au venit in firma dupa primul venit din
-- departamentul lor sa se afiseze numele, venitul si data angajarii

SELECT A.ENAME, A.SAL + NVL(A.COMM, 0) VENIT, A.HIREDATE
	FROM EMP A, EMP SEF
	WHERE A.MGR = SEF.EMPNO
		AND A.SAL + NVL(A.COMM, 0) > SEF.SAL + NVL(SEF.COMM, 0)
		AND A.HIREDATE > (SELECT MIN(B.HIREDATE)
						FROM EMP B
						WHERE B.DEPTNO = A.DEPTNO);

-- angajatul cu al 2lea salariu maxim din firma

SELECT A.ENAME, A.SAL
	FROM EMP A
	WHERE A.SAL = (SELECT MAX(SAL)
			FROM EMP
			WHERE SAL != (SELECT MAX(SAL)
					FROM EMP));

-- al 5lea

SELECT A.ENAME, A.SAL
	FROM EMP A
	WHERE 4 = (SELECT COUNT(DISTINCT B.SAL)
		FROM EMP B
		WHERE B.SAL > A.SAL);

-- primii 5

SELECT A.ENAME, A.SAL
	FROM EMP A
	WHERE 4 >= (SELECT COUNT(DISTINCT B.SAL)
		FROM EMP B
		WHERE B.SAL > A.SAL);

-- care sunt angajatii care au salariile 3, 4, 5

SELECT A.ENAME, A.SAL
	FROM EMP A
	WHERE 4 >= (SELECT COUNT(DISTINCT B.SAL)
			FROM EMP B
			WHERE B.SAL > A.SAL)
		AND 2 <= (SELECT COUNT(DISTINCT B.SAL)
				FROM EMP B
				WHERE B.SAL > A.SAL);

-- sa se afiseze toti angajatii departamentelor research sau sales care s au angajat cu cel putin
-- 3 luni dupa seful lor direct afisand pt fiecare angajat numele, numele sefului, data angajarii sefului
-- data angajarii angajatului, nr de luni intre cei 2, traducerea denumirii departamentului 
-- nu se va tine cont de angajatii care castiga mai putin decat media venitului pe departamentul lor

SELECT A.ENAME, B.ENAME NUME_SEF, A.HIREDATE, B.HIREDATE DATA_SEF, DECODE(A.JOB, 'CLERK', 'FUNCTIONAR', 
			'SALESMAN', 'AGENT VANZARI', 'MANAGER', 'MANAGER', 'ANALYST', 'ANALIST',
			'PRESIDENT', 'SEF') TRADUCERE, MONTHS_BETWEEN(A.HIREDATE, B.HIREDATE) DIFERENTA_LUNI
	FROM EMP A, EMP B
	WHERE A.MGR = B.EMPNO
		AND MONTHS_BETWEEN(A.HIREDATE, B.HIREDATE) >= 3
		AND A.SAL + NVL(A.COMM, 0) > (SELECT AVG(C.SAL + NVL(C.COMM, 0))
										FROM EMP C
										WHERE C.DEPTNO = A.DEPTNO)
		AND (A.DEPTNO = (SELECT DEPTNO FROM DEPT WHERE DNAME = 'RESEARCH')
			OR A.DEPTNO = (SELECT DEPTNO FROM DEPT WHERE DNAME = 'SALES'));

-- sa se selecteze numele angajatului, gradul sau salarial, salariul si salariul maxim din firma

SELECT A.ENAME, B.GRADE, A.SAL, C.MAXS
	FROM EMP A, SALGRADE B, (SELECT MAX(SAL) MAXS FROM EMP) C
	WHERE A.SAL BETWEEN B.LOSAL AND B.HISAL;

-- sa se noteze pt fiecare angajat ce castiga > BLAKE, numele angajatului, salariul, salariul Blake,
-- dieferenta intre salarii si denumirea depart lui Blake

SELECT A.ENAME, A.SAL, BLAKE.SAL, A.SAL - BLAKE.SAL, BLAKE.DNAME
	FROM EMP A, (SELECT B.SAL, C.DNAME
			FROM EMP B JOIN DEPT C ON B.DEPTNO = C.DEPTNO
			WHERE B.ENAME = 'BLAKE') BLAKE
	WHERE A.SAL > BLAKE.SAL;

-- pt fiecare angajat ce nu face parte din depart angajatului cu cel mai mic salariu din firma si nu 
-- s a angajat in luna februarie selectati numele, denumirea depart, numele sefului sau, luna angajarii
-- diferenta dintre venitul sefului si venitul sau si plafonul sau salarial

SELECT A.ENAME, D.DNAME, SEF.ENAME NUME_SEF, EXTRACT(MONTH FROM A.HIREDATE) LUNA_ANGAJARII, 
		SEF.SAL + NVL(SEF.COMM, 0) - A.SAL - NVL(A.COMM, 0) DIFERENTA_VENITURI, S.HISAL
	FROM EMP A, SALGRADE S, DEPT D, EMP SEF
	WHERE A.DEPTNO != (SELECT DEPTNO
				FROM EMP
				WHERE SAL = (SELECT MIN(SAL) FROM EMP))
		AND EXTRACT(MONTH FROM A.HIREDATE) != 2
		AND A.DEPTNO = D.DEPTNO
		AND A.MGR = SEF.EMPNO
		AND A.SAL BETWEEN S.LOSAL AND S.HISAL;
