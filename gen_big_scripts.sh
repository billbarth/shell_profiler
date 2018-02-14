#!/bin/bash

sizes=(10000 50000 100000)

for s in "${sizes[@]}"; do
  fn="test_${s}.sh"
  if [ -f "$fn" ]; then
    rm $fn
  fi
  echo "creating $fn"
  echo "#!/bin/bash" >> $fn
  chmod +x $fn
  for i in $(seq 1 $s); do
    echo "/bin/echo $i" >> $fn
  done
done
