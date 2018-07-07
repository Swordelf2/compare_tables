Dim oExcel, wShell
Dim fso
Dim workDir

Sub Init
	Set wShell = CreateObject("WScript.Shell")
	Set oExcel = CreateObject("Excel.Application") 
	oExcel.DisplayAlerts = FALSE 'to avoid prompts
	
	Set fso = CreateObject("Scripting.FileSystemObject")
	workDir = fso.GetAbsolutePathname(".") & "\tmp"
	If Not fso.FolderExists(workDir) Then fso.CreateFolder(workDir)
End Sub

Function ConvToCsv(id)
	Set oExec=wShell.Exec("mshta.exe ""about:<input type=file id=FILE><script>FILE.click();new ActiveXObject('Scripting.FileSystemObject').GetStandardStream(1).WriteLine(FILE.value);close();resizeTo(0,0);</script>""")
	sFileSelected = oExec.StdOut.ReadLine
	If sFileSelected = Empty Then
		wScript.Echo "wtf man" 'TEMP
		call Cleanup
		wScript.Quit
	End If
	
	Dim oBook, local
	Set oBook = oExcel.Workbooks.Open(sFileSelected)
	local = true
	Dim outName
	outName = workDir & "\" & id & ".csv"
	call oBook.SaveAs(outName, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, local) 'this changed
	oBook.Close False 
	ConvToCsv = outName
	
End Function

Sub Cleanup
	oExcel.Quit
	If fso.FolderExists(workDir) Then fso.DeleteFolder(workDir)
End Sub

call Init
ConvToCsv(0)
ConvToCsv(1)

Dim File0, File1, OutFile, PolicyId0, Price0, PolicyId1, Price1, FullName
File0 = " .\tmp/0.csv "
File1 = " .\tmp/1.csv "
OutFile = " .\out.csv "
PolicyId0 = " 1 "
Price0 = " 2 "
PolicyId1 = " 1 "
Price1 = " 2 "
FullName = " 0 "

wShell.Run(".\compare_tables.exe" & File0 & File1 & OutFile & PolicyId0 & Price0 & PolicyId1 & Price1 & FullName)

'call Cleanup