param([string]$RackPath='C:/Program Files/VCV/Rack2Free/Rack.exe',[switch]$SkipBuild)
$ErrorActionPreference='Stop'
$projectRoot=Split-Path -Parent $PSScriptRoot
if(Get-Process Rack -ErrorAction SilentlyContinue){throw 'Close Rack before the isolated panel test.'}
$env:CHERE_INVOKING='yes';$env:MSYSTEM='MINGW64'
Push-Location -LiteralPath $projectRoot
try {
    if(!$SkipBuild){
        & "$env:LOCALAPPDATA/MetropolixDev/msys64/usr/bin/bash.exe" -lc 'make panel-test-build'
        if($LASTEXITCODE -ne 0){throw 'Panel QA build failed.'}
    }
    $profile=Join-Path $projectRoot 'tmp/quo-vadis-panel-profile'
    $output=Join-Path $projectRoot 'tmp/panel-style-qa'
    $plugin=Join-Path $profile 'plugins-win-x64/MetropolixLab'
    New-Item -ItemType Directory -Path $plugin,$output -Force | Out-Null
    Copy-Item -LiteralPath 'build/panel-test/plugin.dll','plugin.json' -Destination $plugin -Force
    $manifest=Get-Content -LiteralPath 'plugin.json' -Raw | ConvertFrom-Json
    $manifest.modules+=@{slug='AudioQA';name='Internal audio test'}
    $manifest | ConvertTo-Json -Depth 10 | Set-Content -LiteralPath (Join-Path $plugin 'plugin.json') -Encoding utf8NoBOM
    Copy-Item -LiteralPath 'res' -Destination $plugin -Recurse -Force
    '{"windowSize":[1600,950],"windowPos":[30,30],"sampleRate":48000,"showTipsOnLaunch":false,"autoCheckUpdates":false}' | Set-Content -LiteralPath (Join-Path $profile 'settings.json') -Encoding utf8NoBOM
    $qaLog=Join-Path $profile 'log.txt'
    if(Test-Path -LiteralPath $qaLog){Move-Item -LiteralPath $qaLog -Destination (Join-Path $output ('previous-'+[guid]::NewGuid().ToString('N')+'.log'))}
    $patch=Get-Content -LiteralPath 'examples/Hardware-panel-demo.vcv' -Raw | ConvertFrom-Json
    $patch.modules=@($patch.modules | Where-Object {$_.plugin -eq 'MetropolixLab' -and $_.model -eq 'Metropolix'})
    $patch.modules[0].pos=@(0,0);$patch.cables=@();$patch.zoom=2;$patch.gridOffset=@(-1,-.05)
    $patchPath=Join-Path $output 'panel-test.vcv'
    $patch | ConvertTo-Json -Depth 40 | Set-Content -LiteralPath $patchPath -Encoding utf8NoBOM
    '' | Set-Content -LiteralPath (Join-Path $output 'report.txt') -Encoding utf8NoBOM
    $env:METROPOLIX_QA_DIR=$output.Replace('\','/');$env:QUO_VADIS_PANEL_QA='1'
    foreach($reopen in @($false,$true)) {
        if($reopen){$env:QUO_VADIS_PANEL_REOPEN_QA='1';$patchPath=Join-Path $output 'panel-roundtrip.vcv'}
        $arguments=@('-u',('"'+$profile+'"'),'-s',('"'+(Split-Path -Parent $RackPath)+'"'),('"'+$patchPath+'"'))
        $process=Start-Process -FilePath $RackPath -ArgumentList $arguments -WindowStyle Hidden -PassThru
        if(!$process.WaitForExit(45000)){Stop-Process -Id $process.Id;throw 'Panel QA timed out.'}
        $report=Get-Content -LiteralPath (Join-Path $output 'report.txt') -Raw
        $complete=if($reopen){'PASS PANEL REOPEN QA COMPLETE'}else{'PASS PANEL UI QA COMPLETE'}
        if($process.ExitCode -ne 0 -or $report.Contains('FAIL') -or !$report.Contains($complete)){throw ('Panel QA failed: '+$report)}
    }
    Get-Content -LiteralPath (Join-Path $output 'report.txt') | Select-Object -Last 8
} finally {
    Remove-Item Env:METROPOLIX_QA_DIR,Env:QUO_VADIS_PANEL_QA,Env:QUO_VADIS_PANEL_REOPEN_QA -ErrorAction SilentlyContinue
    Pop-Location
}
