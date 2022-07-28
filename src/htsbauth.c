/*
HTTrack Website Copier, Offline Browser for Windows and Unix
Copyright (C) 1998-2018 Xavier Roche and other contributors

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

Important notes:

- We hereby ask people using this source NOT to use it in purpose of grabbing
emails addresses, or collecting any other private information on persons.
This would disgrace our work, and spoil the many hours we spent on it.

Please visit our Website: http://www.httrack.com
*/

/* ------------------------------------------------------------ */
/* File: httrack.c subroutines:                                 */
/*       basic authentication: password storage                 */
/* Author: Xavier Roche                                         */
/* ------------------------------------------------------------ */

/* Internal engine bytecode */
#define HTS_INTERNAL_BYTECODE

#include "htsbauth.h"

/* specific definitions */
#include "htsglobal.h"
#include "htslib.h"
#include "htscore.h"
#include "cJSON\cJSON.h"
#define JS_ERR_BUF_SIZE 1024
#define JS_MAX_BUF_SIZE 1024*1024
#define JS_CJSON_ERROR -123928
#define JS_UNEXPECTED_DATA_TYPE_ERRNO ENOMSG


void DEBUG_COOK_PRINT_EXPANDED(const char* context, t_cookie_expanded* cookie_expanded);
errno_t cjson_SafeReadString(const cJSON* object, const char* string_property, char* buffer, size_t buffer_size);
errno_t cjson_SafeWriteString(const cJSON* object, const char* string_property, const char* string_val);
int save_json_cookies_to_file(FILE* fp, t_cookie* cookie);
errno_t _save_cookie_json(FILE* fp, t_cookie* cookie);
int parse_json_cookies_from_file(FILE* fp, t_cookie* cookie);
errno_t _parse_cookie_json(FILE* fp, t_cookie* out_cookie);

/* END specific definitions */

// gestion des cookie
// ajoute, dans l'ordre
// !=0 : erreur
int cookie_add(t_cookie* cookie, const char* cook_name, const char* cook_value,
	const char* domain, const char* path) {
	t_cookie_expanded expanded;
	if (strlen(cook_value) > sizeof(expanded.cook_value))
		return -1;                  // trop long
	if (strlen(cook_name) > sizeof(expanded.cook_name))
		return -1;                  // trop long
	if (strlen(domain) > sizeof(expanded.domain))
		return -1;                  // trop long
	if (strlen(path) > sizeof(expanded.path))
		return -1;                  // trop long
	strcpy_s(expanded.cook_value, sizeof(expanded.cook_value), cook_value);
	strcpy_s(expanded.cook_name, sizeof(expanded.cook_name), cook_name);
	strcpy_s(expanded.domain, sizeof(expanded.domain), domain);
	strcpy_s(expanded.path, sizeof(expanded.path), path);
	return cookie_addE(cookie, &expanded);
}
int cookie_addE(t_cookie * cookie, t_cookie_expanded * cookie_expanded) {
  char buffer[COOKIE_SINGLE_MAX_SIZE];
  char *a = cookie->data;
  char *insert;
  char cook[COOKIE_VALUE_BUFFER_SIZE*2];
  

  // effacer éventuel cookie en double
  cookie_del(cookie, cookie_expanded->cook_name, cookie_expanded->domain, cookie_expanded->path);
  insert = a;                   // insérer ici
  while(*a) {
    if (strlen(cookie_get(buffer, a, COOK_PARAM_PATH)) < strlen(cookie_expanded->path))        // long. path (le + long est prioritaire)
      a = cookie->data + strlen(cookie->data);  // fin
    else {
      a = strchr(a, '\n');      // prochain champ
      if (a == NULL)
        a = cookie->data + strlen(cookie->data);        // fin
      else
        a++;
      while(*a == '\n')
        a++;
      insert = a;               // insérer ici
    }
  }
  // construction du cookie
  strcpybuff(cook, cookie_expanded->domain);
  strcatbuff(cook, "\t");
  strcatbuff(cook, "TRUE");
  strcatbuff(cook, "\t");
  strcatbuff(cook, cookie_expanded->path);
  strcatbuff(cook, "\t");
  strcatbuff(cook, "FALSE");
  strcatbuff(cook, "\t");
  strcatbuff(cook, "1999999999");
  strcatbuff(cook, "\t");
  strcatbuff(cook, cookie_expanded->cook_name);
  strcatbuff(cook, "\t");
  strcatbuff(cook, cookie_expanded->cook_value);
  strcatbuff(cook, "\n");
  if (!((strlen(cookie->data) + strlen(cook)) < cookie->max_len))
    return -1;                  // impossible d'ajouter
  cookie_insert(insert, cook);
#if DEBUG_COOK
  DEBUG_COOK_PRINT_EXPANDED("add new cookie", cookie_expanded);
  //printf(">>>cook: %s<<<\n",cookie->data);
#endif
  return 0;
}

// effacer cookie si existe
int cookie_del(t_cookie * cookie, const char *cook_name, const char *domain, const char *path) {
  char *a, *b;

  b = cookie_find(cookie->data, cook_name, domain, path);
  if (b) {
    a = cookie_nextfield(b);
    cookie_delete(b, a - b);
#if DEBUG_COOK
    printf("deleted old cookie: %s %s %s\n", cook_name, domain, path);
#endif
  }
  return 0;
}

// Matches wildcard cookie domains that start with a dot
// chk_dom: the domain stored in the cookie (potentially wildcard).
// domain: query domain
static int cookie_cmp_wildcard_domain(const char *chk_dom, const char *domain) {
  const size_t n = strlen(chk_dom);
  const size_t m = strlen(domain);
  const size_t l = n < m ? n : m;
  size_t i;
  for (i = l - 1; i >= 0; i--) {
    if (chk_dom[n - i - 1] != domain[m - i - 1]) {
      return 1;
    }
  }
  if (m < n && chk_dom[0] == '.') {
    return 0;
  }
  else if (m != n) {
    return 1;
  }
  return 0;
}


// rechercher cookie à partir de la position s (par exemple s=cookie.data)
// renvoie pointeur sur ligne, ou NULL si introuvable
// path est aligné à droite et cook_name peut être vide (chercher alors tout cookie)
/* english trans:
	// search cookie from position s (for example s=cookie.data)
	// returns pointer to row, or NULL if not found
	// path is right-aligned and cook_name can be empty (then search for any cookie)
*/
// .doubleclick.net     TRUE    /       FALSE   1999999999      id      A
char *cookie_find(char *s, const char *cook_name, const char *domain, const char *path) {
  char buffer[COOKIE_VALUE_BUFFER_SIZE];
  char *a = s;

  while(*a) {
    int t;

    if (strnotempty(cook_name) == 0)
      t = 1;                    // accepter par défaut
    else
      t = (strcmp(cookie_get(buffer, a, COOK_PARAM_NAME), cook_name) == 0);   // tester si même nom
    if (t) {                    // même nom ou nom qualconque
      //
      const char *chk_dom = cookie_get(buffer, a, COOK_PARAM_DOMAIN); // domaine concerné par le cookie

      if ((strlen(chk_dom) <= strlen(domain) &&
        strcmp(chk_dom, domain + strlen(domain) - strlen(chk_dom)) == 0) ||
        !cookie_cmp_wildcard_domain(chk_dom, domain)) {  // même domaine
          //
        const char *chk_path = cookie_get(buffer, a, COOK_PARAM_PATH);    // chemin concerné par le cookie

        if (strlen(chk_path) <= strlen(path)) {
          if (strncmp(path, chk_path, strlen(chk_path)) == 0) {       // même chemin
            return a;
          }
        }
      }
    }
    a = cookie_nextfield(a);
  }
  return NULL;
}

// renvoie prochain champ
char *cookie_nextfield(char *a) {
  char *b = a;

  a = strchr(a, '\n');          // prochain champ
  if (a == NULL)
    a = b + strlen(b);          // fin
  else
    a++;
  while(*a == '\n')
    a++;
  return a;
}

// lire cookies.txt
// lire également (Windows seulement) les *@*.txt (cookies IE copiés)
// !=0 : erreur
int cookie_load(t_cookie * cookie, const char *fpath, const char *name) {
  char file_path_buffer[CATBUFF_SIZE];
  t_cookie_expanded cookie_expanded;

  //  cookie->data[0]='\0';
  BOOL json_mode = strendwith_(name, ".json");

  // Fusionner d'abord les éventuels cookies IE
#ifdef _WIN32
  {
	  if (!json_mode) {
		  WIN32_FIND_DATAA find;
		  HANDLE h;
		  char pth[MAX_PATH + 32];

		  strcpybuff(pth, fpath);
		  strcatbuff(pth, "*@*.txt");
		  h = FindFirstFileA((char*)pth, &find);
		  if (h != INVALID_HANDLE_VALUE) {
			  do {
				  if (!(find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					  if (!(find.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)) {
						  FILE* fp = fopen(fconcat(file_path_buffer, sizeof(file_path_buffer), fpath, find.cFileName), "rb");

						  if (fp) {
							  char dummy[COOKIE_PARAM_BUFFER_SIZE];
							  
							  //
							  lien_adrfil af;   // chemin (/)
							  int cookie_merged = 0;

							  //
							  // Read all cookies
							  while (!feof(fp)) {
								  cookie_expanded = EmptyExpandedCookieStruct;
								  dummy[0] = af.adr[0] = af.fil[0] = '\0';

								  linput(fp, cookie_expanded.cook_name, sizeof(cookie_expanded.cook_name));
								  if (!feof(fp)) {
									  linput(fp, cookie_expanded.cook_value, sizeof(cookie_expanded.cook_value));
									  if (!feof(fp)) {
										  int i;

										  linput(fp, cookie_expanded.path, sizeof(cookie_expanded.path));//technically this is domain and path so could be truncating to only path length but its quite long
										  /* Read 6 other useless values */
										  for (i = 0; !feof(fp) && i < 6; i++) {
											  linput(fp, dummy, 500);
										  }
										  if (strnotempty(cookie_expanded.cook_name)
											  && strnotempty(cookie_expanded.cook_value)
											  && strnotempty(cookie_expanded.path)) {
											  if (ident_url_absolute(cookie_expanded.path, &af) >= 0) {
												  strcpy_s(cookie_expanded.domain, sizeof(cookie_expanded.domain), af.adr);
												  strcpy_s(cookie_expanded.path, sizeof(cookie_expanded.path), af.fil);
												  cookie_addE(cookie, &cookie_expanded);
												  cookie_merged = 1;
											  }
										  }
									  }
								  }
							  }
							  fclose(fp);
							  if (cookie_merged)
								  remove(fconcat(file_path_buffer, sizeof(file_path_buffer), fpath, find.cFileName));
						  }                   // if fp
					  }
			  } while (FindNextFileA(h, &find));
			  FindClose(h);
		  }
	  }
  }
#endif

  // Ensuite, cookies.txt
  {
    FILE *fp = fopen(fconcat(file_path_buffer, sizeof(file_path_buffer), fpath, name), "rb");
	
    if (fp) {
		if (!json_mode) {
			char BIGSTK line[COOKIE_SINGLE_MAX_SIZE];

			while ((!feof(fp)) && (((int)strlen(cookie->data)) < cookie->max_len)) {
				rawlinput(fp, line, sizeof(line));
				if (strnotempty(line)) {
					if (strlen(line) < COOKIE_SINGLE_MAX_SIZE) {
						if (line[0] != '#') {
							cookie_expanded = cookie_getE(line);
							DEBUG_COOK_PRINT_EXPANDED("File read cookie", &cookie_expanded);
							cookie_addE(cookie, &cookie_expanded);
						}
					}
				}
			}
		}
		else
			parse_json_cookies_from_file(fp, cookie);
		
      fclose(fp);
      return 0;
    }
  }
  return -1;
}

void DEBUG_COOK_PRINT_EXPANDED(const char * context, t_cookie_expanded * cookie_expanded) {
#ifdef  DEBUG_COOK
	printf_s("%s: name: %s val: %s dom: %s path: %s\n", context, cookie_expanded->cook_name, cookie_expanded->cook_value, cookie_expanded->domain, cookie_expanded->path);
#endif //  DEBUG_COOK
}
errno_t cjson_SafeReadString(const cJSON* object, const char* string_property, char* buffer, size_t buffer_size) {
	const cJSON* tmp = cJSON_GetObjectItem(object, string_property);
	if (tmp == NULL)
		return JS_CJSON_ERROR;
	if (!cJSON_IsString(tmp))
		return JS_UNEXPECTED_DATA_TYPE_ERRNO;
	strcpy_s(buffer, buffer_size, tmp->valuestring);
	return 0;
}
errno_t cjson_SafeWriteString(cJSON* object, const char* string_property, const char* string_val) {
	
	cJSON* tmp = cJSON_CreateString(string_val);
	if (tmp == NULL)
		return ENOMEM;
	if (!cJSON_AddItemToObject(object, string_property, tmp)) {
		cJSON_Delete(tmp);
		return ENOMEM;
	}
	return 0;
}
int save_json_cookies_to_file(FILE* fp, t_cookie* to_save) {

	char* b = to_save->data;
	errno_t err = 0;
	cJSON* cookies = cJSON_CreateArray();
	if (cookies == NULL)
		goto fail;
	cJSON* cookie = NULL;
	t_cookie_expanded cookie_expanded;

	while (b != NULL && b[0] != '\0') {
		cookie_expanded = cookie_getE(b);
		cookie = cJSON_CreateObject();
		if (cookie == NULL)
			goto fail;
		if (!cJSON_AddItemToArray(cookies, cookie))
			goto fail;
		err = cjson_SafeWriteString(cookie, "name",cookie_expanded.cook_name);
		if (err)
			goto fail;
		err = cjson_SafeWriteString(cookie, "value", cookie_expanded.cook_value);
		if (err)
			goto fail;
		err = cjson_SafeWriteString(cookie, "domain", cookie_expanded.domain);
		if (err)
			goto fail;
		err = cjson_SafeWriteString(cookie, "path", cookie_expanded.path);
		if (err)
			goto fail;
		 
		b = cookie_nextfield(b);
	}
	fputs( cJSON_Print(cookies), fp);
	goto cleanup;

fail:
	if (cookies != NULL)//technically cookie as wel lcoudl not be added but additemtoarray shouldwnt fail
		cJSON_Delete(cookies);
	if (! err)
		err = 1;
cleanup:
	fclose(fp);
	return err;
}
int parse_json_cookies_from_file(FILE* fp, t_cookie* cookie) {
	errno_t res = _parse_cookie_json(fp, cookie);
	if (res != 0) {
		if (res == JS_CJSON_ERROR) {
			fprintf(stderr, "CJSON Parse Error at: %s", cJSON_GetErrorPtr());
		}
		else {
			char errmsg[JS_ERR_BUF_SIZE];
			strerror_s(errmsg, JS_ERR_BUF_SIZE, res);
			fprintf(stderr, "Error parsing json: %s\n", errmsg);
		}
		return -1;
	}

	return 0;
}
errno_t _parse_cookie_json(FILE* fp, t_cookie* out_cookie) {
	errno_t err;
	cJSON* cookies = NULL;
	char* buffer = NULL;
	fseek(fp, 0L, SEEK_END);
	const long numBytes = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	buffer = (char*)calloc(numBytes, sizeof(char));
	if (buffer == NULL)
		return ENOMEM;

	fread_s(buffer, sizeof(char) * numBytes, sizeof(char), numBytes, fp);

	t_cookie_expanded cookie_expanded;
	cJSON* cookie = NULL;

	cookies = cJSON_Parse(buffer);


	if (cookies == NULL)
		goto cjson_fail;
	cJSON_ArrayForEach(cookie, cookies) {
		if (cookie == NULL)
			goto cjson_fail;
		err = cjson_SafeReadString(cookie, "name", cookie_expanded.cook_name, COOKIE_PARAM_BUFFER_SIZE);
		if (err)
			goto fail;
		err = cjson_SafeReadString(cookie, "value", cookie_expanded.cook_value, COOKIE_PARAM_BUFFER_SIZE);
		if (err)
			goto fail;
		err = cjson_SafeReadString(cookie, "domain", cookie_expanded.domain, COOKIE_PARAM_BUFFER_SIZE);
		if (err)
			goto fail;
		err = cjson_SafeReadString(cookie, "path", cookie_expanded.path, COOKIE_PARAM_BUFFER_SIZE);
		if (err)
			goto fail;
		
#if DEBUG_COOK
		DEBUG_COOK_PRINT_EXPANDED("Read a json cookie", cookie_expanded);
#endif
		cookie_addE(out_cookie, &cookie_expanded);

		
	}
	goto cleanup;

cjson_fail:
	err = JS_CJSON_ERROR;
	goto cleanup;

fail:
	goto cleanup;
cleanup:
	cJSON_Delete(cookies);
	if (buffer != NULL)
		free(buffer);
	if (fp != NULL)
		fclose(fp);
	return err;
}



// écrire cookies.txt
// !=0 : erreur
int cookie_save(t_cookie * cookie, const char *name) {
  char catbuff[CATBUFF_SIZE];

  if (strnotempty(cookie->data)) {
    
    FILE *fp = fopen(fconv(catbuff, sizeof(catbuff), name), "wb");
	BOOL json_mode = strendwith_(name, ".json");

	if (fp) {
		if (!json_mode ) {
			char BIGSTK line[COOKIE_VALUE_BUFFER_SIZE];

			char* a = cookie->data;

			fprintf(fp,
				"# HTTrack Website Copier Cookie File" LF
				"# This file format is compatible with Netscape cookies" LF);
			do {
				a += binput(a, line, COOKIE_VALUE_BUFFER_SIZE);
				fprintf(fp, "%s" LF, line);
			} while (strnotempty(line));
			fclose(fp);
			return 0;
		}
		else {
			save_json_cookies_to_file(fp, cookie);
		}
	}
  } else
    return 0;
  return -1;
}


// insertion chaine ins avant s
void cookie_insert(char *s, const char *ins) {
  char *buff;

  if (strnotempty(s) == 0) {    // rien à faire, juste concat
    strcatbuff(s, ins);
  } else {
    buff = (char *) malloct(strlen(s) + 1);
    if (buff) {
      strcpybuff(buff, s);      // copie temporaire
      strcpybuff(s, ins);       // insérer
      strcatbuff(s, buff);      // copier
      freet(buff);
    }
  }
}

// destruction chaine dans s position pos
void cookie_delete(char *s, size_t pos) {
  char *buff;

  if (strnotempty(s + pos) == 0) {      // rien à faire, effacer
    s[0] = '\0';
  } else {
    buff = (char *) malloct(strlen(s + pos) + 1);
    if (buff) {
      strcpybuff(buff, s + pos);        // copie temporaire
      strcpybuff(s, buff);      // copier
      freet(buff);
    }
  }
}


t_cookie_expanded cookie_getE(const char* cookie_base) {
	t_cookie_expanded expanded;
	char buffer[sizeof(expanded.cook_value)];//i doubt anything ever gets longer than value
	strcpy_s(expanded.cook_value, sizeof(expanded.cook_value), cookie_get(buffer, cookie_base, COOK_PARAM_VALUE));
	strcpy_s(expanded.cook_name, sizeof(expanded.cook_name), cookie_get(buffer, cookie_base, COOK_PARAM_NAME));
	strcpy_s(expanded.domain, sizeof(expanded.domain), cookie_get(buffer, cookie_base, COOK_PARAM_DOMAIN));
	strcpy_s(expanded.path, sizeof(expanded.path), cookie_get(buffer, cookie_base, COOK_PARAM_PATH));
	return expanded;
}

// renvoie champ param de la chaine cookie_base
// ex: cookie_get("ceci est<tab>un<tab>exemple",1) renvoi "un"
const char *cookie_get(char *buffer, const char *cookie_base, COOK_PARAM_t param) {
  const char *limit;

  while(*cookie_base == '\n')
    cookie_base++;
  limit = strchr(cookie_base, '\n');
  if (!limit)
    limit = cookie_base + strlen(cookie_base);
  if (limit) {
    if (param) {
      int i;

      for(i = 0; i < param; i++) {
        if (cookie_base) {
          cookie_base = strchr(cookie_base, '\t');      // prochain tab
          if (cookie_base)
            cookie_base++;
        }
      }
    }
    if (cookie_base) {
      if (cookie_base < limit) {
        const char *a = cookie_base;

        while((*a) && (*a != '\t') && (*a != '\n'))
          a++;
        buffer[0] = '\0';
        strncatbuff(buffer, cookie_base, (int) (a - cookie_base));
        return buffer;
      } else
        return "";
    } else
      return "";
  } else
    return "";
}

// fin cookies

// -- basic auth --

/* déclarer un répertoire comme possédant une authentification propre */
int bauth_add(t_cookie * cookie, const char *adr, const char *fil, const char *auth) {
  char buffer[HTS_URLMAXSIZE * 2];

  if (cookie) {
    if (!bauth_check(cookie, adr, fil)) {       // n'existe pas déja
      bauth_chain *chain = &cookie->auth;
      char *prefix = bauth_prefix(buffer, adr, fil);

      /* fin de la chaine */
      while(chain->next)
        chain = chain->next;
      chain->next = (bauth_chain *) calloc(sizeof(bauth_chain), 1);
      if (chain->next) {
        chain = chain->next;
        chain->next = NULL;
        strcpybuff(chain->auth, auth);
        strcpybuff(chain->prefix, prefix);
        return 1;
      }
    }
  }
  return 0;
}

/* tester adr et fil, et retourner authentification si nécessaire */
/* sinon, retourne NULL */
char *bauth_check(t_cookie * cookie, const char *adr, const char *fil) {
  char buffer[HTS_URLMAXSIZE * 2];

  if (cookie) {
    bauth_chain *chain = &cookie->auth;
    char *prefix = bauth_prefix(buffer, adr, fil);

    while(chain) {
      if (strnotempty(chain->prefix)) {
        if (strncmp(prefix, chain->prefix, strlen(chain->prefix)) == 0) {
          return chain->auth;
        }
      }
      chain = chain->next;
    }
  }
  return NULL;
}

char *bauth_prefix(char *prefix, const char *adr, const char *fil) {
  char *a;

  strcpybuff(prefix, jump_identification_const(adr));
  strcatbuff(prefix, fil);
  a = strchr(prefix, '?');
  if (a)
    *a = '\0';
  if (strchr(prefix, '/')) {
    a = prefix + strlen(prefix) - 1;
    while(*a != '/')
      a--;
    *(a + 1) = '\0';
  }
  return prefix;
}
