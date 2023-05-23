/* sa se selecteze pt fiecare angajat care nu face parte din depart lui BLAKE si care 
   castiga un venit mai mic decat seful sau
   afiseaza: numele, denumirea departamentului, venitul sau, numele sefului sau si diferenta 
   veniturilor sef - angajat 2 metode diferite */

SELECT A.ENAME, D.DNAME, A.SAL + NVL(A.COMM, 0) VENIT, SEF.ENAME, SEF.SAL + NVL(SEF.COMM, 0) - A.SAL + NVL(A.COMM, 0) DIFERENTA_VENIT 
	FROM EMP A, EMP SEF, EMP BLAKE, DEPT D
	WHERE A.MGR = SEF.EMPNO
		AND A.DEPTNO = D.DEPTNO 
		AND BLAKE.ENAME = 'BLAKE'
		AND A.DEPTNO != BLAKE.DEPTNO
		AND A.SAL + NVL(A.COMM, 0) < SEF.SAL + NVL(SEF.COMM, 0);

SELECT A.ENAME, D.DNAME, A.SAL + NVL(A.COMM, 0) VENIT, SEF.ENAME, SEF.SAL + NVL(SEF.COMM, 0) - A.SAL + NVL(A.COMM, 0) DIFERENTA_VENIT 
	FROM EMP A JOIN EMP SEF ON A.MGR = SEF.EMPNO, EMP BLAKE, DEPT D
	WHERE A.DEPTNO = D.DEPTNO 
		AND BLAKE.ENAME LIKE 'BLAKE'
		AND A.DEPTNO != BLAKE.DEPTNO
		AND A.SAL + NVL(A.COMM, 0) < SEF.SAL + NVL(SEF.COMM, 0);
