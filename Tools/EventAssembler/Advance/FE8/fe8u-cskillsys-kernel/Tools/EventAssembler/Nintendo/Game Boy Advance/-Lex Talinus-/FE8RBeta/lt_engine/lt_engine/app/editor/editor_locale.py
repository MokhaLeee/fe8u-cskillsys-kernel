import gettext

def init_locale():
    current_locale = 'en_US'
    locale_path = 'locale/'
    translation_module = gettext.translation(
        domain='messages',
        localedir=locale_path,
        languages=[current_locale]
    )
    translation_module.install()  # Magically make the _ function globally available
