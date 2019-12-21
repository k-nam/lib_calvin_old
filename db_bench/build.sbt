name := "rainbow-table-db"

version := "1.0"

scalaVersion := "2.12.8"

lazy val akkaVersion = "2.5.19"

libraryDependencies ++= Seq(
  "org.scalatest" %% "scalatest" % "3.0.5" % "test",
  "mysql" % "mysql-connector-java" % "5.1.12"
)
