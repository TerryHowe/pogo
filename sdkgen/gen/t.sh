INPUT=$1
NAME=$(basename $1)
CLASS=$(dirname $1)
if [ -z "${NAME}" -o -z "${CLASS}" ]
then
  echo 'gimme a name'
  exit 1
fi
NAMEO=`echo ${NAME:0:1} | tr  '[a-z]' '[A-Z]'`${NAME:1}
BASE_PATH=$(echo ${NAME} | sed -e 's/_/-/g')
CAMEL=$(gsed -e 's/_\([a-z]\)\([a-z]*\)/\U\1\L\2/g' -e 's/^\([a-z]\)/\U\1/g' <<< "${NAME}")

git checkout master
git branch -D ${NAME}
git checkout -b ${NAME}

CNT=1
FILE=openstack/${CLASS}/${NAME}.py
TEST=openstack/tests/${CLASS}/test_${NAME}.py
sed -e "s/Asdf/${NAMEO}/g" \
    -e "s/Camel/${CAMEL}/g" \
    -e "s/asdf/${NAME}/g" \
    -e "s/baser/${BASE_PATH}s/g" gen/resource >${FILE}
sed -e "s/Asdf/${NAMEO}/g" \
    -e "s/Camel/${CAMEL}/g" \
    -e "s/asdf/${NAME}/g" gen/test_resource >${TEST}
sort ${INPUT} |
while read FIELD FROM TYPE
do
  if [ -z "${FROM}" ]
  then
    FROM=$FIELD
  fi
  FIELD=$(echo $FIELD | sed -e 's/:/_/g')
  if [ "${FIELD}" != 'id' ]
  then
    echo "    ${FIELD} = resource.prop('${FROM}')" >>${FILE}
  fi
  if [ -z "${TYPE}" ]
  then
    TYPE="'$CNT'"
  fi
  echo "    '${FROM}': ${TYPE}," >>${TEST}
  CNT=$(expr $CNT + 1)
done

sed -e "s/Asdf/${NAMEO}/g" \
    -e "s/Camel/${CAMEL}/g" \
    -e "s/asdf/${NAME}/g" \
    -e "s/baser/${BASE_PATH}s/g" gen/test_resource_tail >>${TEST}
sort ${INPUT} |
while read FIELD FROM TYPE
do
  if [ -z "${FROM}" ]
  then
    FROM=$FIELD
  fi
  FIELD=$(echo $FIELD | sed -e 's/:/_/g')
  LINE="        self.assertEqual(EXAMPLE['${FROM}'], sot.${FIELD})"
  CC=$(echo "$LINE" | wc -c)
  if [ $CC -ge 80 ]
  then
    echo "        self.assertEqual(EXAMPLE['${FROM}']," >>${TEST}
    echo "                         sot.${FIELD})" >>${TEST}
  else
    echo "$LINE" >>${TEST}
  fi
done

git add $FILE
git add $TEST
git commit -a -m "${CLASS} ${NAME} resource"
