/*
 * Copyright 2014 Open Source Robotics Foundation, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Defines all of the Package manifest related data structures and functions.
 *
 * Example:
 *
 *     Pkg_Package *pkg = Pkg_InitPackage();
 *     int ret = Pkg_ParsePackageManifest("/path/to/package.xml", pkg);
 *     if (ret)
 *     {
 *         // Error handling
 *     }
 *     Pkg_PrintPackage(pkg);
 *     // Use the pkg some other way...
 *     // Free the Pkg_Package
 *     Pkg_FreePackage(pkg);
 */

/* Struct to capture a person for use in listing of maintainers and authors */
typedef struct Pkg_PersonList
{
    char *name;
    char *email;
    struct Pkg_PersonList *next;
} Pkg_PersonList;

/* Initializes a Pkg_PersonList, call before using a Pkg_PersonList */
Pkg_PersonList *
Pkg_InitPersonList();

/* Frees a Pkg_PersonList
 *
 * Freeing is done recursively by traveling down the next pointers.
 * If you only want to free this Person, unset and update next pointers first.
 */
void
Pkg_FreePersonList(Pkg_PersonList *person_list);

/* Struct to capture the list names of license used in a package */
typedef struct Pkg_LicenseList
{
    char *license;
    struct Pkg_LicenseList *next;
} Pkg_LicenseList;

/* Initializes a Pkg_LicenseList, call be using a Pkg_LicenseList */
Pkg_LicenseList *
Pkg_InitLicenseList();

/* Frees a Pkg_LicenseList
 *
 * Freeing is done recursively by traveling down the next pointers.
 * If you only want to free this License, unset and update next pointers first.
 */
void
Pkg_FreeLicenseList(Pkg_LicenseList *license_list);

/* Enum to define the possible URL types */
typedef enum Pkg_URLType
{
    PKG_URL_NOT_SET,
    PKG_URL_WEBSITE,
    PKG_URL_BUGTRACKER,
    PKG_URL_REPOSITORY
} Pkg_URLType;

/* Struct to capture a list of urls given by a package manifest */
typedef struct Pkg_URLList
{
    char *url;
    Pkg_URLType type;
    struct Pkg_URLList *next;
} Pkg_URLList;

/* Initializes Pkg_URLList, call before using a Pkg_URLList */
Pkg_URLList *
Pkg_InitURLList();

/* Frees a Pkg_URLList
 *
 * Freeing is done recursively by traveling down the next pointers.
 * If you only want to free this url, unset and update next pointers first.
 */
void
Pkg_FreeURLList(Pkg_URLList *url_list);

/* Struct to capture version information as major.minor.patch integers */
typedef struct Pkg_Version
{
    unsigned int major;
    unsigned int minor;
    unsigned int patch;
} Pkg_Version;

/* Struct to capture a list of typeless dependencies from a package manifest */
typedef struct Pkg_DependencyList
{
    char *name;
    Pkg_Version *version_lt;
    Pkg_Version *version_lte;
    Pkg_Version *version_eq;
    Pkg_Version *version_gt;
    Pkg_Version *version_gte;
    struct Pkg_DependencyList *next;
} Pkg_DependencyList;

/* Initializes Pkg_DependencyList, call before using a Pkg_DependencyList */
Pkg_DependencyList *
Pkg_InitDependencyList();

/* Frees a Pkg_DependencyList
 *
 * Freeing is done recursively by traveling down the next pointers.
 * If you only want to free this dep, unset and update next pointers first.
 */
void
Pkg_FreeDependencyList(Pkg_DependencyList *dep_list);

/* Struct to capture the contents of a package manifest */
typedef struct Pkg_Package
{
    /* package.xml format version */
    unsigned int package_format;
    /* filename of the package.xml this was created from */
    char *filename;
    /* package name */
    char *name;
    /* version triplet {major, minor, patch} */
    Pkg_Version version;
    /* abi version triplet {major, minor, patch} */
    Pkg_Version abi_version;
    /* package description */
    char *description;
    /* package maintainers */
    Pkg_PersonList *maintainers;
    /* package licenses */
    Pkg_LicenseList *licenses;
    /* package urls */
    Pkg_URLList *urls;
    /* package authors */
    Pkg_PersonList *authors;
    /* buildtool_depends */
    Pkg_DependencyList *buildtool_depends;
    /* build_depends */
    Pkg_DependencyList *build_depends;
    /* run_depends */
    Pkg_DependencyList *run_depends;
    /* test_depends */
    Pkg_DependencyList *test_depends;
    /* Export Section */
    char *exports;
} Pkg_Package;

/* Initializes a Pkg_Package struct, call before using a Pkg_Package */
Pkg_Package *
Pkg_InitPackage();

/* Frees a Pkg_Package object, recursively freeing any contained structs */
void
Pkg_FreePackage(Pkg_Package *pkg);

/* Prints the contents of a Pkg_Package struct */
void
Pkg_PrintPackage(Pkg_Package *pkg);

/* Parses a package manifest file and puts the result in a Pkg_Package */
int
Pkg_ParsePackageManifest(const char *path, Pkg_Package *pkg);
