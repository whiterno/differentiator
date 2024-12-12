_DEF_CMD(ADD, "+", {
    return _ADD(_DIFF_L, _DIFF_R);
})

_DEF_CMD(SUB, "-", {
    return _SUB(_DIFF_L, _DIFF_R);
})

_DEF_CMD(MULT, "*",  {
    return _ADD( _MULT(_DIFF_L, _COPY_R),
                 _MULT(_COPY_L, _DIFF_R)
                );
})

_DEF_CMD(DIV, "/",  {
    return _DIV(
                _SUB( _MULT(_DIFF_L, _COPY_R),
                      _MULT(_COPY_L, _DIFF_R)
                    ),
                _MULT(_COPY_R, _COPY_R)
                );
})

_DEF_CMD(POW, "^",  {
    return diffPow(root);
})

_DEF_CMD(EXP, "exp",  {
    return _MULT(
                 _EXP(_COPY_R),
                 _DIFF_R
                );
})

_DEF_CMD(LN, "ln",  {
    return _MULT(
                _DIV(_NUM(1), _COPY_R),
                _DIFF_R
                );
})

_DEF_CMD(LOG, "log",  {
    return diff(_DIV(
                    _LN(_COPY_R),
                    _LN(_COPY_L)
                    )
                );
})

_DEF_CMD(SIN, "sin",  {
    return _MULT(
                _COS(_COPY_R),
                _DIFF_R
                );
})

_DEF_CMD(COS, "cos",  {
    return _MULT(_NUM(-1),
                 _MULT(
                        _SIN(_COPY_R),
                        _DIFF_R
                      )
                );
})

_DEF_CMD(TAN, "tan",  {
    return _MULT(
                _DIV(
                    _NUM(1),
                    _POW(_COS(_COPY_R), _NUM(2))
                    ),
                _DIFF_R
                );
})

_DEF_CMD(COT, "cot",  {
    return _MULT(
                _DIV(
                    _NUM(1),
                    _POW(_SIN(_COPY_R), _NUM(2))
                    ),
                _MULT(_NUM(-1), _DIFF_R)
                );
})
