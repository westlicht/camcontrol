@echo off
rem ==========================================================================
rem Product: QP/C application buld script for PC-Lint(TM), Standard C compiler
rem Last Updated for Version: 4.0.01
rem Date of the Last Update:  Nov 10, 2008
rem
rem                    Q u a n t u m     L e a P s
rem                    ---------------------------
rem                    innovating embedded systems
rem
rem Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
rem
rem This software may be distributed and modified under the terms of the GNU
rem General Public License version 2 (GPL) as published by the Free Software
rem Foundation and appearing in the file GPL.TXT included in the packaging of
rem this file. Please note that GPL Section 2[b] requires that all works based
rem on this software must also be made publicly available under the terms of
rem the GPL ("Copyleft").
rem
rem Alternatively, this software may be distributed and modified under the
rem terms of Quantum Leaps commercial licenses, which expressly supersede
rem the GPL and are specifically designed for licensees interested in
rem retaining the proprietary status of their code.
rem
rem Contact information:
rem Quantum Leaps Web site:  http://www.quantum-leaps.com
rem e-mail:                  info@quantum-leaps.com
rem ==========================================================================
setlocal

set PC_LINT_DIR=c:\tools\Lint

set QPN_INCDIR=..\..\..\..\..\include
set APP_DIR=..

if "%1"=="" (
    set LINTFLAGS=+v -zero -i%PC_LINT_DIR%\lnt std.lnt -si4 -ss2 -sp4 -i%QPN_INCDIR%
)

set COMP_INCDIR=c:\tools\tcpp101\INCLUDE

@echo on
%PC_LINT_DIR%\lint-nt %LINTFLAGS% opt_app.lnt -i%COMP_INCDIR% -i%APP_DIR% -os(lint_app.txt) %APP_DIR%\*.c
@echo off

endlocal
