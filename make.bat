set MSBuild="%SystemRoot%\Microsoft.NET\Framework\v4.0.30319\msbuild.exe"

%MSBuild% ctodo.vcxproj /p:Configuration=Release
tests.bat