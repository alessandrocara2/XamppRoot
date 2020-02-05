
$ErrorActionPreference = 'Stop';
$toolsDir   = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"
$url        = 'https://github.com/alessandrocara2/XamppRoot/releases/download/1.0/xampproot.exe'

$packageArgs = @{
  packageName    = $env:ChocolateyPackageName
  unzipLocation  = $toolsDir
  fileType       = 'exe'
  url64bit       = $url
  FileFullPath   = "$toolsDir\xampproot.exe"

  softwareName   = 'xampproot*'

  checksum64     = '750BF63DE8970CF7DCDED9C69C3620E3957050C4DA065007B44208341F42099A'
  checksumType64 = 'sha256'

  validExitCodes = @(0, 3010, 1641)
  silentArgs     = '/VERYSILENT'
}

Get-ChocolateyWebFile @packageArgs