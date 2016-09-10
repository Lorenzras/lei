for %%f in (Images\*.jpg) do (
	echo %%~nf
        lei "Images\%%~nf.jpg"
       
)

pause