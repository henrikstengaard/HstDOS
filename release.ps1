# Release
# -------
# Author: Henrik Noerfjand Stengaard
# Date:   2020-04-12
#
# Powershell script to start DOSBox and run Turbo C v3.0 to compile HstDOS and build a release.

# paths
$rootPath = Resolve-Path '.'
$srcPath = Join-Path $rootPath -ChildPath 'src'
$testPath = Join-Path $rootPath -ChildPath 'test'
$releasePath = Join-Path $rootPath -ChildPath '.release'
$hstDosPath = Join-Path $releasePath -ChildPath 'HstDOS'
$menuPath = Join-Path $hstDosPath -ChildPath 'Menu'

# find dosbox path
$dosBoxPath = Get-ChildItem -Path ${env:ProgramFiles(x86)} | Where-Object { $_ -match 'dosbox' } | Select-Object -First 1

# fail, if dosbox is not installed
if (!$dosBoxPath)
{
    throw "DOSBox is not installed"
}

# dosbox.exe
$dosBoxExe = Join-Path $dosBoxPath.Fullname -ChildPath 'DOSBox.exe'

# fail, if dosbox.exe is not installed
if (!(Test-Path $dosBoxExe))
{
    throw "DOSBox is not installed"
}

# start dosbox
$dosBoxArgs = "-conf dosbox.conf -c ""pause"" -c ""exit"""
$dosBoxProcess = Start-Process $dosBoxExe -ArgumentList $dosBoxArgs -WorkingDirectory $rootPath -Wait -NoNewWindow -PassThru
if ($dosBoxProcess.ExitCode -ne 0)
{
	Write-Host ("Error: DosBox failed with exit code {0}!" -f $dosBoxProcess.ExitCode) -ForegroundColor 'Red'
	exit 1
}

# remove release directory, if it exists
if (Test-Path -Path $releasePath)
{
    Remove-Item -Path $releasePath -Recurse -Force
}

# create menu directory
mkdir -Path $menuPath | Out-Null

# copy hstdos.exe, hd.bat and autoexec.hd
Copy-Item (Join-Path $srcPath -ChildPath 'hstdos.exe') -Destination "$hstDosPath\\hstdos.exe"
Copy-Item (Join-Path $srcPath -ChildPath 'hd.bat') -Destination "$hstDosPath\\hd.bat"
Copy-Item (Join-Path $srcPath -ChildPath 'autoexec.hd') -Destination "$releasePath\\autoexec.hd"

# copy menu
Copy-Item -Path "$testPath\\*" -Recurse -Destination $menuPath