for %%f in (*.jpg) do (
	echo %%~nf
        lei "%%~nf.jpg"
       
)

pause