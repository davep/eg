  @Echo Off

  Del *.bak
  For %%a in (*.ngs) Do ngc %%a
  ngml eg.mnu
  Del *.ngo
