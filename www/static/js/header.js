function setThemeDark() {
  localStorage.theme = "dark";
  $("body").toggleClass("light");
}

function setThemeLight() {
  localStorage.theme = "light";
  $("body").toggleClass("light");
}

function changeTheme() {
  if (localStorage.theme == "dark") {
    setThemeLight();
  } else {
    setThemeDark();
  }
}

$(function () {
  if (localStorage.theme == "light") {
    setThemeLight();
  }
  $(".navbar-nav")
    .append("<li><a>Change theme</a></li>")
    .click(function () {
      changeTheme();
    });
  $("[role=complementary] h1")
    .text("")
    .html('<img src="/static/img/Prologin2024.png" style="width:100%;" />');
});
