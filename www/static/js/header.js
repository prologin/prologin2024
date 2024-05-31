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
  button = $("<li><a>Change theme</a></li>");
  button.click(function () {
    changeTheme();
  });

  $(".navbar-nav").append(button);
  $("[role=complementary] h1")
    .text("")
    .html('<img src="/static/img/Prologin2024.png" style="width:100%;" />');
});
