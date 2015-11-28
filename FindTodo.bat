@echo off

pushd Source
findstr /i /n /c:"// TODO" *.h *.hpp *.inl *.cpp
popd
