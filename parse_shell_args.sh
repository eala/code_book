# Parse command-line options:

premake_args=""

with_system_premake5=false
without_nvtt=false
with_system_nvtt=false
with_system_mozjs=false
enable_atlas=true

for i in "$@"
do
  case $i in
    --with-system-premake5 ) with_system_premake5=true ;;
    --without-nvtt ) without_nvtt=true; premake_args="${premake_args} --without-nvtt" ;;
    --with-system-nvtt ) with_system_nvtt=true; premake_args="${premake_args} --with-system-nvtt" ;;
    --with-system-mozjs ) with_system_mozjs=true; premake_args="${premake_args} --with-system-mozjs" ;;
    --enable-atlas ) enable_atlas=true ;;
    --disable-atlas ) enable_atlas=false ;;
    -j* ) JOBS=$i ;;
    # Assume any other --options are for Premake
    --* ) premake_args="${premake_args} $i" ;;
  esac
done

if [ "$enable_atlas" = "true" ]; then
  premake_args="${premake_args} --atlas"
fi
