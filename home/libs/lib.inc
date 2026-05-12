DECLARE ProgramSegmentPrefix LITERALLY 'STRUCTURE (
	skip1    (2)BYTE,
	topOfMem    WORD,
	skip2  (28h)BYTE,
	envs    SELECTOR,
	skip3  (52h)BYTE,
	argLen      BYTE,
	argBuf (127)BYTE
)';

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

DECLARE _PRINTHEXBODY LITERALLY '
	DECLARE (hdigit, shift, i) BYTE;
	shift = SIZE(n) * 8 - 4;

	IF n = 0 THEN CALL dos_std_out(''0'');
	ELSE DO i = 1 TO SIZE(n)*2;
		hdigit = SHR(n, shift) AND 0FH;
		IF (hdigit >= 0) AND (hdigit <= 9) THEN
			CALL dos_std_out(hdigit + ''0'');
		ELSE
			CALL dos_std_out(hdigit - 10 + ''A'');
		shift = shift - 4;
	END;

	CALL dos_std_out(''H'');
';

PrintByteHex: PROCEDURE(n);
	DECLARE n BYTE;
	_PRINTHEXBODY;
END PrintByteHex;

PrintWordHex: PROCEDURE(n);
	DECLARE n WORD;
	_PRINTHEXBODY;
END PrintWordHex;

PrintDwordHex: PROCEDURE(n);
	DECLARE n DWORD;
	_PRINTHEXBODY;
END PrintDwordHex;


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
	DECLARE eps REAL DATA(1.e-6);
	DECLARE x REAL, ix INTEGER;
	DECLARE i BYTE;

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

DECLARE _READBODY LITERALLY '
	DECLARE buf STRUCTURE (req BYTE, read BYTE, chars(21) BYTE, cr BYTE);
	DECLARE (i, c) BYTE, sign INTEGER;

	buf.req = LENGTH(buf.chars);
	CALL dos_buffered_std_in(@buf); CALL Println;

	sign = 1; n = 0;
	DO i = 0 TO buf.read;
		c = buf.chars(i);
		IF (i = 0) AND (c = ''-'') THEN sign = -1;
		IF (c >= ''0'') AND (c <= ''9'') THEN n = n * 10 + cc - ''0'';
	END;
';

ReadDword: PROCEDURE DWORD;
	DECLARE cc LITERALLY 'c';
	DECLARE n DWORD;
	_READBODY;
	RETURN n;
END ReadDword;

ReadInt: PROCEDURE INTEGER;
	DECLARE cc LITERALLY 'INT(c)';
	DECLARE n INTEGER;
	_READBODY;
	RETURN sign * n;
END ReadInt;

ReadWord: PROCEDURE WORD;
	DECLARE cc LITERALLY 'c';
	DECLARE n WORD;
	_READBODY;
	RETURN n;
END ReadWord;

ReadReal: PROCEDURE REAL;
	DECLARE buf STRUCTURE (req BYTE, read BYTE, chars(21) BYTE, cr BYTE);
	DECLARE (i, c, state, power) BYTE, sign INTEGER;
	DECLARE (num, denum) REAL;

	buf.req = LENGTH(buf.chars);
	CALL dos_buffered_std_in(@buf); CALL Println;

	state = power = 0; sign = 1; num = 0.; denum = 0.;
	DO i = 0 TO buf.read;
		c = buf.chars(i);
		IF (i = 0) AND (c = '-') THEN sign = -1;
		IF (c = '.') THEN DO;
			power = buf.read - i;
			state = 1;
		END;
		IF (c >= '0') AND (c <= '9') THEN DO CASE state;
			/* case 0: */ num   = num   * 10. + FLOAT(INT(c - '0'));
			/* case 1: */ denum = denum * 10. + FLOAT(INT(c - '0'));
		END;
	END;

	DO i = 1 TO power-1;
		denum = denum / 10.;
	END;

	RETURN FLOAT(sign) * (num + denum);
END ReadReal;

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

Min: PROCEDURE(a, b) WORD;
	DECLARE (a, b) WORD;
	IF a < b THEN RETURN a;
	ELSE RETURN b;
END Min;
