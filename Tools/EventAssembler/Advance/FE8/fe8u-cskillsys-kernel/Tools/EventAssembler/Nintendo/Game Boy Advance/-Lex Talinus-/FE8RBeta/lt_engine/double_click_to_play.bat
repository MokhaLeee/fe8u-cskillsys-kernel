for /d %%d in (*.*) do (
    set test=%%d
)

set name=%test%
cd ./%name%
.\%name%.exe