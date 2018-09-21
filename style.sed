/}$/ {
  N
  s/\n\s+else/ else/
}
s/\}else/} else/g
s/else\{/else {/g
s/(if|while|for)\(/\1 (/g
s/\)\{/) {/g
s/([^\/ ])\* ([^\/])/\1 *\2/g
/^[^ #\/\}\{]/ {
  s/ +\{//
  a\
{
}
s/\s+\n$/\n/
