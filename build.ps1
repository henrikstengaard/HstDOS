# Build
# -----
# Author: Henrik Noerfjand Stengaard
# Date:   2020-04-07
#
# Powershell script to start DOSBox and run Turbo C v3.0 to compile HstDOS.

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

# start dosbox.exe
& $dosBoxExe -conf dosbox.conf