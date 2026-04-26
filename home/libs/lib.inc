/* NOTE(anton2920): this exceeds theoretical limit on string constants by 56 (311 vs. 255). */
DECLARE _PRINTBODY LITERALLY '
	DECLARE (i, count) BYTE, digits(20) BYTE;
	IF n = 0 THEN DO;
		CALL dos_std_out(''0'');
		RETURN;
	END;

	count = 0;
	DO WHILE n > 0;
		digits(LAST(digits)-count) = (n MOD 10) + ''0'';
		n = n / 10;
		count = count + 1;
	END;

	DO i = LAST(digits)-count+1 TO LAST(digits);
		CALL dos_std_out(digits(i));
	END
';

PrintWord: PROCEDURE(n);
	DECLARE n WORD;
	_PRINTBODY;
END PrintWord;

PrintDword: PROCEDURE(n);
	DECLARE n DWORD;
	_PRINTBODY;
END PrintDword;

PrintInt: PROCEDURE(n);
	DECLARE n INTEGER;
	IF n < 0 THEN DO;
		n = -n;
		CALL dos_std_out('-');
	END;
	CALL PrintWord(UNSIGN(n));
END PrintInt;

PrintReal: PROCEDURE(x);
	DECLARE x REAL, ix INTEGER;
	DECLARE eps REAL DATA(1.e-6);
	DECLARE i WORD;

	IF x < 0. THEN DO;
		x = ABS(x);
		CALL dos_std_out('-');
	END;

	ix = FIX(x);
	CALL PrintInt(ix);
	x = (x - FLOAT(ix)) * 10.;

	IF x > eps THEN DO;
		CALL dos_std_out('.');

		i = 6;
		DO WHILE (x > eps) AND (i > 0);
			ix = FIX(x);
			CALL PrintInt(ix);
			x = (x - FLOAT(ix)) * 10.;
			i = i - 1;
		END;
	END;
END PrintReal;

Println: PROCEDURE;
	CALL dos_std_string_out(@(13, 10, '$'));
END Println;

ReadChar: PROCEDURE BYTE;
	DECLARE buf STRUCTURE (req BYTE, read BYTE, chars(1) BYTE, cr BYTE);

	buf.req = 2;
	CALL dos_buffered_std_in(@buf); CALL Println;

	RETURN buf.chars(0);
END ReadChar;

ReadWord: PROCEDURE WORD;
	DECLARE n WORD, (i, c) BYTE;
	DECLARE buf STRUCTURE (req BYTE, read BYTE, chars(20) BYTE, cr BYTE);

	buf.req = 20;
	CALL dos_buffered_std_in(@buf); CALL Println;

	n = 0;
	DO i = 0 TO buf.read;
		c = buf.chars(i);
		IF (c >= '0') AND (c <= '9') THEN n = n * 10 + c - '0';
	END;

	RETURN n;
END ReadWord;

DECLARE _seed WORD INITIAL(1);
Srand: PROCEDURE(seed);
	DECLARE seed WORD;
	_seed = seed;
END Srand;

Rand: PROCEDURE WORD;
	DECLARE a WORD DATA (16807), c WORD DATA (0), m WORD DATA (0FFFFh);
	_seed = (_seed * a + c) MOD m;
	RETURN _seed;
END Rand;
