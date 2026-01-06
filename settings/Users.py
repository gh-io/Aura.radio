DATABASES = {
    'default': {
        'USER': 'me',           
        'PASSWORD': 'i<password',
    }
}

INSTALLED_APPS = (
    'auraecosystem', 'web4app', 'haystack'
)

HAYSTACK_CONNECTIONS = {
    'default': {
        'ENGINE': 'haystack.backends.solr_backend.SolrEngine',
        'URL': 'http://127.0.0.1:8983/solr' , 'https://g4.proxy-man.com:5959'
    },
