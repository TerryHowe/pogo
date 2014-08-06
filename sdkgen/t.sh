INPUT=$1
IFS=/ read SDKGEN SERVICE VERSION RESOURCE <<< "${INPUT}"
BIG_SERVICE=$(sed -e 's/_\([a-z]\)\([a-z]*\)/\U\1\L\2/g' -e 's/^\([a-z]\)/\U\1/g' <<< "${SERVICE}")
DESTER=$(sed -e 's/\..*//g' <<< "$VERSION")
if [ -z "${VERSION}" -o -z "${SERVICE}" ]
then
  echo 'gimme a name'
  exit 1
fi
if [ -z "${RESOURCE}" ]
then
  RESOURCE=$VERSION
  VERSION=''
fi
NAMEO=`echo ${RESOURCE:0:1} | tr  '[a-z]' '[A-Z]'`${RESOURCE:1}
BASE_PATH=$(echo ${RESOURCE} | sed -e 's/_/-/g')
CAMEL=$(sed -e 's/_\([a-z]\)\([a-z]*\)/\U\1\L\2/g' -e 's/^\([a-z]\)/\U\1/g' <<< "${RESOURCE}")

git checkout master
git branch -D ${RESOURCE}
git checkout -b ${RESOURCE}

CNT=1
SRCDIR="openstack/${SERVICE}/${DESTER}"
TSTDIR="openstack/tests/${SERVICE}/${DESTER}"
mkdir -p $SRCDIR
mkdir -p $TSTDIR
FILE=${SRCDIR}/${RESOURCE}.py
TEST=${TSTDIR}/test_${RESOURCE}.py
sed -e "s/Asdf/${NAMEO}/g" \
    -e "s/Camel/${CAMEL}/g" \
    -e "s/swervice/${SERVICE}/g" \
    -e "s/Swervice/${BIG_SERVICE}/g" \
    -e "s/Dester/${DESTER}/g" \
    -e "s/Vershun/${VERSION}/g" \
    -e "s/asdf/${RESOURCE}/g" \
    -e "s/baser/${BASE_PATH}s/g" ${SDKGEN}/resource >${FILE}
sed -e "s/Asdf/${NAMEO}/g" \
    -e "s/Camel/${CAMEL}/g" \
    -e "s/swervice/${SERVICE}/g" \
    -e "s/Swervice/${BIG_SERVICE}/g" \
    -e "s/Dester/${DESTER}/g" \
    -e "s/Vershun/${VERSION}/g" \
    -e "s/asdf/${RESOURCE}/g" ${SDKGEN}/test_resource >${TEST}
sort ${INPUT} |
while read FIELD FROM TYPE
do
  if [ -z "${FROM}" ]
  then
    FROM=$FIELD
  fi
  FIELD=$(echo $FIELD | sed -e 's/:/_/g')
  if [ "${TYPE}" == 'True' -o "${TYPE}" == 'False' ]
  then
    PROP_TYPE=', type=bool'
  else
    PROP_TYPE=''
  fi
  if [ "${FIELD}" != 'id' ]
  then
    echo "    ${FIELD} = resource.prop('${FROM}'${PROP_TYPE})" >>${FILE}
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
    -e "s/swervice/${SERVICE}/g" \
    -e "s/Swervice/${BIG_SERVICE}/g" \
    -e "s/Dester/${DESTER}/g" \
    -e "s/Vershun/${VERSION}/g" \
    -e "s/asdf/${RESOURCE}/g" \
    -e "s/baser/${BASE_PATH}s/g" ${SDKGEN}/test_resource_tail >>${TEST}
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
git commit -a -m "${SERVICE}/${VERSION} ${RESOURCE} resource"
echo $FILE $TEST
