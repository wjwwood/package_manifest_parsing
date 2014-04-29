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

#include <assert.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#if !defined(LIBXML_TREE_ENABLED) || !defined(LIBXML_OUTPUT_ENABLED)
# error LibXML2 not compiled with tree support
#endif

#include <package_manifest_parsing/pkg.h>

/* Pkg_PersonList Functions */
Pkg_PersonList *
Pkg_InitPersonList()
{
    Pkg_PersonList *person_list = \
        (Pkg_PersonList *)malloc(sizeof(Pkg_PersonList));
    person_list->email = NULL;
    person_list->name = NULL;
    person_list->next = NULL;
    return person_list;
}

void
Pkg_FreePersonList(Pkg_PersonList *person_list)
{
    if (person_list->email) free(person_list->email);
    if (person_list->name) free(person_list->name);
    if (person_list->next) Pkg_FreePersonList(person_list->next);
    free(person_list);
}

/* Pkg_LicenseList Functions */
Pkg_LicenseList *
Pkg_InitLicenseList()
{
    Pkg_LicenseList *license_list = \
        (Pkg_LicenseList *)malloc(sizeof(Pkg_LicenseList));
    license_list->license = NULL;
    license_list->next = NULL;
    return license_list;
}

void
Pkg_FreeLicenseList(Pkg_LicenseList *license_list)
{
    if (license_list->license) free(license_list->license);
    if (license_list->next) Pkg_FreeLicenseList(license_list->next);
    free(license_list);
}

/* Pkg_URLList Functions */
Pkg_URLList *
Pkg_InitURLList()
{
    Pkg_URLList *url_list = \
        (Pkg_URLList *)malloc(sizeof(Pkg_URLList));
    url_list->url = NULL;
    url_list->type = PKG_URL_NOT_SET;
    url_list->next = NULL;
    return url_list;
}

void
Pkg_FreeURLList(Pkg_URLList *url_list)
{
    if (url_list->url) free(url_list->url);
    if (url_list->next) Pkg_FreeURLList(url_list->next);
    free(url_list);
}

/* Pkg_DependencyList Functions */
Pkg_DependencyList *
Pkg_InitDependencyList()
{
    Pkg_DependencyList *dep_list = \
        (Pkg_DependencyList *)malloc(sizeof(Pkg_DependencyList));
    dep_list->name = NULL;
    dep_list->version_lt = NULL;
    dep_list->version_lte = NULL;
    dep_list->version_eq = NULL;
    dep_list->version_gt = NULL;
    dep_list->version_gte = NULL;
    dep_list->next = NULL;
    return dep_list;
}

void
Pkg_FreeDependencyList(Pkg_DependencyList *dep_list)
{
    if (dep_list->name) free(dep_list->name);
    if (dep_list->version_lt) free(dep_list->version_lt);
    if (dep_list->version_lte) free(dep_list->version_lte);
    if (dep_list->version_eq) free(dep_list->version_eq);
    if (dep_list->version_gt) free(dep_list->version_gt);
    if (dep_list->version_gte) free(dep_list->version_gte);
    if (dep_list->next) Pkg_FreeDependencyList(dep_list->next);
    free(dep_list);
}

/* Pkg_InitPackage Functions */
Pkg_Package *
Pkg_InitPackage()
{
    Pkg_Package *pkg = (Pkg_Package *)malloc(sizeof(Pkg_Package));
    pkg->package_format = 0;
    pkg->filename = NULL;
    pkg->name = NULL;
    pkg->version.major = 0;
    pkg->version.minor = 0;
    pkg->version.patch = 0;
    pkg->abi_version.major = 0;
    pkg->abi_version.minor = 0;
    pkg->abi_version.patch = 0;
    pkg->description = NULL;
    pkg->maintainers = NULL;
    pkg->licenses = NULL;
    pkg->urls = NULL;
    pkg->authors = NULL;
    pkg->buildtool_depends = NULL;
    pkg->build_depends = NULL;
    pkg->run_depends = NULL;
    pkg->test_depends = NULL;
    pkg->exports = NULL;
    return pkg;
}

void
Pkg_FreePackage(Pkg_Package *pkg)
{
    if (pkg->filename) free(pkg->filename);
    if (pkg->name) free(pkg->name);
    if (pkg->description) free(pkg->description);
    if (pkg->maintainers) Pkg_FreePersonList(pkg->maintainers);
    if (pkg->licenses) Pkg_FreeLicenseList(pkg->licenses);
    if (pkg->urls) Pkg_FreeURLList(pkg->urls);
    if (pkg->authors) Pkg_FreePersonList(pkg->authors);
    if (pkg->buildtool_depends) Pkg_FreeDependencyList(pkg->buildtool_depends);
    if (pkg->build_depends) Pkg_FreeDependencyList(pkg->build_depends);
    if (pkg->run_depends) Pkg_FreeDependencyList(pkg->run_depends);
    if (pkg->test_depends) Pkg_FreeDependencyList(pkg->test_depends);
    if (pkg->exports) free(pkg->exports);
    free(pkg);
}

static inline void
depPrintHelper(Pkg_DependencyList *dep)
{
    while (dep)
    {
        printf("  %s\n", dep->name);
        if (dep->version_eq)
            printf("   version_eq: %d.%d.%d\n",
                   dep->version_eq->major,
                   dep->version_eq->minor,
                   dep->version_eq->patch);
        if (dep->version_lt)
            printf("   version_lt: %d.%d.%d\n",
                   dep->version_lt->major,
                   dep->version_lt->minor,
                   dep->version_lt->patch);
        if (dep->version_lte)
            printf("   version_lte: %d.%d.%d\n",
                   dep->version_lte->major,
                   dep->version_lte->minor,
                   dep->version_lte->patch);
        if (dep->version_gt)
            printf("   version_gt: %d.%d.%d\n",
                   dep->version_gt->major,
                   dep->version_gt->minor,
                   dep->version_gt->patch);
        if (dep->version_gte)
            printf("   version_gte: %d.%d.%d\n",
                   dep->version_gte->major,
                   dep->version_gte->minor,
                   dep->version_gte->patch);
        dep = dep->next;
    }
}

void
Pkg_PrintPackage(Pkg_Package * pkg)
{
    printf("Package:\n");
    printf(" name: %s\n", pkg->name);
    printf(" version: %d.%d.%d\n",
           pkg->version.major,
           pkg->version.minor,
           pkg->version.patch);
    printf(" description: %s\n", pkg->description);
    printf(" maintainers:\n");
    Pkg_PersonList *maintainer = pkg->maintainers;
    while (maintainer)
    {
        if (maintainer->email)
            printf("  %s <%s>\n", maintainer->name, maintainer->email);
        else
            printf("  %s\n", maintainer->name);
        maintainer = maintainer->next;
    }
    printf(" licenses:\n");
    Pkg_LicenseList *license = pkg->licenses;
    while (license)
    {
        printf("  %s\n", license->license);
        license = license->next;
    }
    printf(" urls:\n");
    Pkg_URLList *url = pkg->urls;
    while (url)
    {
        printf("  %s", url->url);
        switch (url->type)
        {
            case PKG_URL_WEBSITE:
                printf(" (website)\n");
                break;
            case PKG_URL_BUGTRACKER:
                printf(" (bugtracker)\n");
                break;
            case PKG_URL_REPOSITORY:
                printf(" (repository)\n");
                break;
            case PKG_URL_NOT_SET:
            default:
                printf("\n");
                break;
        }
        url = url->next;
    }
    printf(" authors:\n");
    Pkg_PersonList *author = pkg->authors;
    while (author)
    {
        if (author->email)
            printf("  %s <%s>\n", author->name, author->email);
        else
            printf("  %s\n", author->name);
        author = author->next;
    }
    Pkg_DependencyList *dep = pkg->buildtool_depends;
    if (dep)
    {
        printf(" buildtool_depends:\n");
        depPrintHelper(dep);
    }
    dep = pkg->build_depends;
    if (dep)
    {
        printf(" build_depends:\n");
        depPrintHelper(dep);
    }
        dep = pkg->run_depends;
    if (dep)
    {
        printf(" run_depends:\n");
        depPrintHelper(dep);
    }
    dep = pkg->test_depends;
    if (dep)
    {
        printf(" test_depends:\n");
        depPrintHelper(dep);
    }
    if (pkg->exports)
    {
        printf(" export:\n");
        printf("  %s\n", pkg->exports);
    }
}

static inline int
str_is_only_zeros(const char *str)
{
    for (int i = 0; i < strlen(str); ++i)
    {
        if ('0' == str[i])
        {
            return 0;
        }
    }
    return 1;
}

/*
 * Returns the next xml element type node as a xmlNode* or NULL if none
 */
static inline xmlNode *
getNextElementNode(xmlNode *node)
{
    xmlNode *element;

    for (element = node->next; element; element = element->next)
    {
        if (XML_ELEMENT_NODE == element->type)
        {
            return element;
        }
    }

    return NULL;
}

static inline char *
getContent(xmlNode *node, const char * tag_name, const char * path)
{
    xmlChar *tag_content = xmlNodeGetContent(node);
    if (!tag_content)
    {
        fprintf(stderr, "No content in <%s> tag of %s\n", tag_name, path);
        return NULL;
    }
    char *result = strdup((char *)tag_content);
    if (!result)
    {
        fprintf(stderr,
                "Failed to strdup content of '<%s>' of %s\n",
                tag_name,
                path);
        return NULL;
    }
    return result;
}

static inline int
parseVersion(const char *version_str, Pkg_Version *version)
{
    if (sscanf(version_str, "%d.%d.%d",
               &version->major,
               &version->minor,
               &version->patch) != 3)
    {
        return 0;
    }
    return 1;
}

static inline int
handleDepend(
    Pkg_DependencyList **dep_list,
    xmlNode *curr,
    const char *tag_name,
    const char *path)
{
    Pkg_DependencyList *dep;
    if (*dep_list)
    {
        dep = *dep_list;
        while (dep->next) dep = dep->next;
        dep->next = Pkg_InitDependencyList();
        dep = dep->next;
    }
    else
    {
        *dep_list = Pkg_InitDependencyList();
        dep = *dep_list;
    }
    dep->name = getContent(curr, tag_name, path);
    if (!dep->name) return 1;
    const char *version_attrs[] = {
        "version_lt",
        "version_lte",
        "version_eq",
        "version_gt",
        "version_gte"
    };
    Pkg_Version **versions[] = {
        &dep->version_lt,
        &dep->version_lte,
        &dep->version_eq,
        &dep->version_gt,
        &dep->version_gte
    };
    for (int i = 0; i < sizeof(version_attrs)/sizeof(version_attrs[0]); ++i)
    {
        const char *attr = version_attrs[i];
        char *version_str = (char *)xmlGetProp(curr, (xmlChar *)attr);
        if (version_str)
        {
            Pkg_Version *ver = (Pkg_Version *)malloc(sizeof(Pkg_Version));
            if (!parseVersion(version_str, ver))
            {
                fprintf(
                    stderr,
                    "Failed to parse version from the '%s' attribute of the "
                    "'<%s>%s</%s>' tag in %s: %s\n",
                    attr, tag_name, dep->name, tag_name, path, version_str);
                free(ver);
                return 1;
            }
            *(versions[i]) = ver;
        }
    }
    return 0;
}

int
Pkg_ParsePackageManifest(const char *path, Pkg_Package *pkg)
{
    /* Assert a path */
    assert(path);

    /* Setup xml structs */
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    /* Test for ABI compatability */
    LIBXML_TEST_VERSION

    /* Try to read in the xml file given */
    doc = xmlReadFile(path, NULL, 0);

    /* If the file cannot be opened, error */
    if (doc == NULL) {
        fprintf(stderr, "Failed to load package manifest %s\n", path);
        return 1;
    }

    /* Put the path into the pkg's filename attribute */
    pkg->filename = strdup(path);
    assert(pkg->filename);

    /* Get the root element */
    root_element = xmlDocGetRootElement(doc);

    /* Search for the <package> tag */
    xmlNode *pkg_node;

    for (pkg_node = root_element; pkg_node; pkg_node = pkg_node->next)
    {
        if (XML_ELEMENT_NODE == pkg_node->type)
        {
            if (0 == strncmp("package", (char *)pkg_node->name, 7))
            {
                break;
            }
            /* Otherwise it is an unknown tag */
            fprintf(stderr,
                    "Unknown tag '<%s>' in %s\n",
                    (char *)pkg_node->name,
                    path);
            return 1;
        }
    }

    /* If <package> not found, error */
    if (!pkg_node)
    {
        fprintf(stderr, "Failed to find <package> tag in %s\n", path);
        return 1;
    }

    /* Assert that there is only one <package> tag */
    int package_found = 0;
    for (xmlNode *node = root_element; node; node = node->next)
    {
        if (XML_ELEMENT_NODE == node->type)
        {
            if (package_found)
            {
                /* Otherwise it is an unknown tag */
                fprintf(
                    stderr,
                    "Found toplevel tag '<%s>' in %s, "
                    "but only one toplevel <package> tag is allowed\n",
                    (char *)node->name,
                    path);
                return 1;
            }
            if (0 == strncmp("package", (char *)node->name, 7))
            {
                package_found = 1;
            }
        }
    }

    /* Look for format attribute of <package> tag */
    xmlChar *package_format = xmlGetProp(pkg_node, (xmlChar *)"format");
    /* Assume format version 1 if not found */
    pkg->package_format = 1;
    if (package_format)
    {
        /* The string is only '0's then set it to zero */
        if (0 != str_is_only_zeros((char *)package_format))
        {
            pkg->package_format = 0;
        }
        else
        {
            /* Otherwise use atoi to determine the number if it is a number */
            int pkg_format_num = atoi((char *)package_format);
            if (0 == pkg_format_num)
            {
                /* If atoi returns 0, then the str was not a number */
                /* We know this because we checked for it being zero */
                /* explicitly before choosing to call atoi */
                /* In which case this is an error */
                fprintf(
                    stderr,
                    "Invalid value in <package> tag's version attribute: %s\n",
                    package_format);
                return 1;
            }
            pkg->package_format = pkg_format_num;
        }
    }
    /* We don't support anything but package version 1 */
    /* So if the version isn't 1, error */
    if (pkg->package_format != 1)
    {
        fprintf(
            stderr,
            "This parser cannot parse package manifests of version %u\n",
            pkg->package_format);
        return 1;
    }

    /* Iterate over all of the tags inside of the <package> tag */
    xmlNode *curr;
    for (curr = pkg_node->children; curr; curr = getNextElementNode(curr))
    {
        char *tag_name = (char *)curr->name;
        if (0 == strcmp("name", tag_name))
        {
            pkg->name = getContent(curr, tag_name, path);
            if (!pkg->name) return 1;
        } else
        if (0 == strcmp("version", tag_name))
        {
            char *version_str = getContent(curr, tag_name, path);
            if (!version_str) return 1;
            if (!parseVersion(version_str, &pkg->version))
            {
                fprintf(stderr, "Invalid <version> tag: '%s'\n", version_str);
                return 1;
            }
        } else
        if (0 == strcmp("description", tag_name))
        {
            pkg->description = getContent(curr, tag_name, path);
            if (!pkg->description) return 1;
        } else
        if (0 == strcmp("maintainer", tag_name))
        {
            Pkg_PersonList *maintainer;
            if (pkg->maintainers)
            {
                maintainer = pkg->maintainers;
                while (maintainer->next) maintainer = maintainer->next;
                maintainer->next = Pkg_InitPersonList();
                maintainer = maintainer->next;
            }
            else
            {
                pkg->maintainers = Pkg_InitPersonList();
                maintainer = pkg->maintainers;
            }
            maintainer->name = getContent(curr, tag_name, path);
            if (!maintainer->name) return 1;
            maintainer->email = (char *)xmlGetProp(curr,
                                                   (xmlChar *)"email");
        } else
        if (0 == strcmp("license", tag_name))
        {
            Pkg_LicenseList *license;
            if (pkg->licenses)
            {
                license = pkg->licenses;
                while (license->next) license = license->next;
                license->next = Pkg_InitLicenseList();
                license = license->next;
            }
            else
            {
                pkg->licenses = Pkg_InitLicenseList();
                license = pkg->licenses;
            }
            license->license = getContent(curr, tag_name, path);
            if (!license->license) return 1;
        } else
        if (0 == strcmp("url", tag_name))
        {
            Pkg_URLList *url;
            if (pkg->urls)
            {
                url = pkg->urls;
                while (url->next) url = url->next;
                url->next = Pkg_InitURLList();
                url = url->next;
            }
            else
            {
                pkg->urls = Pkg_InitURLList();
                url = pkg->urls;
            }
            url->url = getContent(curr, tag_name, path);
            if (!url->url) return 1;
            char *url_type = (char *)xmlGetProp(curr, (xmlChar *)"type");
            if (url_type)
            {
                if (0 == strcmp("website", url_type))
                {
                    url->type = PKG_URL_WEBSITE;
                } else
                if (0 == strcmp("bugtracker", url_type))
                {
                    url->type = PKG_URL_BUGTRACKER;
                } else
                if (0 == strcmp("repository", url_type))
                {
                    url->type = PKG_URL_REPOSITORY;
                }
                else
                {
                    fprintf(stderr,
                            "Unkown url type '%s' in %s\n", url_type, path);
                    return 1;
                }
            }
            else
            {
                url->type = PKG_URL_NOT_SET;
            }
        } else
        if (0 == strcmp("author", tag_name))
        {
            Pkg_PersonList *author;
            if (pkg->authors)
            {
                author = pkg->authors;
                while (author->next) author = author->next;
                author->next = Pkg_InitPersonList();
                author = author->next;
            }
            else
            {
                pkg->authors = Pkg_InitPersonList();
                author = pkg->authors;
            }
            author->name = getContent(curr, tag_name, path);
            if (!author->name) return 1;
            author->email = (char *)xmlGetProp(curr, (xmlChar *)"email");
        } else
        if (0 == strcmp("buildtool_depend", tag_name))
        {
            if (handleDepend(&pkg->buildtool_depends, curr, tag_name, path))
                return 1;
        } else
        if (0 == strcmp("build_depend", tag_name))
        {
            if (handleDepend(&pkg->build_depends, curr, tag_name, path))
                return 1;
        } else
        if (0 == strcmp("run_depend", tag_name))
        {
            if (handleDepend(&pkg->run_depends, curr, tag_name, path))
                return 1;
        } else
        if (0 == strcmp("test_depend", tag_name))
        {
            if (handleDepend(&pkg->test_depends, curr, tag_name, path))
                return 1;
        } else
        if (0 == strcmp("export", tag_name))
        {
            xmlBufferPtr buffer = xmlBufferCreate();
            int bytes = xmlNodeDump(buffer, doc, curr, 0, 1);
            if (-1 == bytes)
            {
                fprintf(stderr,
                        "Failed to dump contents of <export> in %s\n",
                        path);
                return 1;
            }
            pkg->exports = strndup((char *)buffer->content, buffer->size);
        } else
        if (0 == strcmp("text", tag_name))
        {
            /* pass */
        }
        else
        {
            fprintf(stderr, "Unknown tag '<%s>' in %s\n", tag_name, path);
            char *content = getContent(curr, tag_name, path);
            fprintf(stderr, "Unknown tag content: '%s'\n", content);
            free(content);
        }
    }

    /* Cleanup */
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 0;
}
